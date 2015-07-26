// See the file "COPYING" in the main distribution directory for copyright.

#include <algorithm>

#include "NetVar.h"
#include "File.h"
#include "OSFinger.h"
#include "Event.h"

#include "analyzer/protocol/pia/PIA.h"
#include "analyzer/protocol/tcp/TCP.h"
#include "analyzer/protocol/tcp/TCP_Reassembler.h"

#include "TCPRS.h"
#include "TCPRS_Endpoint.h"

#include "tcprs.bif.h"

using namespace analyzer::tcp;

#define TOO_LARGE_SEQ_DELTA 1048576

#define INTERNAL_USE_TCPANALYZER true

TCPRS_Analyzer::TCPRS_Analyzer(Connection* conn)
: TCP_ApplicationAnalyzer("TCPRS", conn)
{
}

TCPRS_Analyzer::~TCPRS_Analyzer()
{
    delete orig;
    delete resp;
}

void TCPRS_Analyzer::Init()
{
  TCP_ApplicationAnalyzer::Init();

  measurement_location = NO_LOCATION_DATA;
  max_confidence = 0;
  location_confidence = 0.0;
  sample_count = 0;

  orig = new TCPRS_Endpoint(TCP()->Orig(), this);
  resp = new TCPRS_Endpoint(TCP()->Resp(), this);

  orig->setOrig(true);
  resp->setOrig(false);

  orig->setPeer(resp);
  resp->setPeer(orig);

  UsesTSOption = false;
  sack_in_use = false;
  spurious_rexmit_count = 0;
}

void TCPRS_Analyzer::Done()
{
    TCP_ApplicationAnalyzer::Done();

    //If the analyzer did not observe the syns of the connection, then we don't
    //  want to summarize data from this connection because it could be misleading
    bool bad_connection = /*TCP()->IsPartial(); */ !((orig->getMSS() > 0) && (resp->getMSS() > 0));

    //If the analyzer did not observe the minimum number of rtt samples for each
    //  side of the connection, then this connection should be omitted.
    bad_connection |= !(orig->getValidRTTSampleCount() >= MINIMUM_SAMPLE_COUNT &&
                        resp->getValidRTTSampleCount() >= MINIMUM_SAMPLE_COUNT );

    //bad_connection |= !TCP()->BothClosed();

    if( !bad_connection ) {
        //Retrieve summaries
        orig->examinePotentialSpuriousRexmits();
        resp->examinePotentialSpuriousRexmits();
    }
    
    ConfigSummary();
}

void TCPRS_Analyzer::ConfigSummary() {
    if( conn_config ) {
        val_list* vl = new val_list;
        //If the analyzer did not observe the syns of the connection, then we don't
        //  want to summarize data from this connection because it could be misleading
        bool bad_connection =  /*TCP()->IsPartial() || */ !((orig->getMSS() > 0) && (resp->getMSS() > 0)) ||
                               !(orig->getValidRTTSampleCount() >= MINIMUM_SAMPLE_COUNT &&
                                resp->getValidRTTSampleCount() >= MINIMUM_SAMPLE_COUNT );// ||
                                //TCP()->BothClosed();

        vl->append(BuildConnVal());
        vl->append(new Val(current_timestamp, TYPE_TIME));
        vl->append(new Val(TSOptionEnabled(), TYPE_BOOL));          //Were timestamps enabled?
        vl->append(new Val(bad_connection, TYPE_BOOL));             //Was this a bad connection?
        vl->append(new Val(sack_in_use, TYPE_BOOL));                //Did the connection actually use sack?
        vl->append(new Val(orig->isSACKEnabled(), TYPE_BOOL));        //Did client offer sack?
        vl->append(new Val(resp->isSACKEnabled(), TYPE_BOOL));        //Did server offer sack?

        TCP()->ConnectionEvent(conn_config, vl);
    }
}

void TCPRS_Analyzer::DeliverPacket(int len, const u_char* data, bool is_orig,
                                uint64 seq, const IP_Hdr* ip, int caplen)
{
  //Gets Header and delivers packet to app analyzer
  TCP_ApplicationAnalyzer::DeliverPacket(len, data, is_orig, seq, ip, caplen);

  TCPRS_Endpoint* endpoint = is_orig ? orig : resp;

  endpoint->DeliverSegment(len,data, is_orig, seq, ip, caplen);
}

int TCPRS_Analyzer::GetLikelyTTLDiff(int ttl)
{
  // common default TTLs are 64, 128, 255.  if we start using more
  // than three default TTLs, i'll fix this method to use a proper list
    int default_ttl = GetLikelyDefaultTTL(ttl);
    if (ttl < 0 || default_ttl == -1)
        return -1;
    return default_ttl - ttl;
}

int TCPRS_Analyzer::GetLikelyDefaultTTL(int ttl) {
    if(ttl <= 64)
        return 64;
    else if (ttl <= 128)
        return 128;
    else if (ttl <= 255)
        return 255;

    return -1;

}

void TCPRS_Analyzer::EstimateMeasurementLocation()
{
    double rtt1 = orig->getPathRTTEstimate();
    double rtt2 = resp->getPathRTTEstimate();

    //Returns a positive number if it is closer to the orig than resp,
    //  negative if closer to resp than orig
    double ChangeInConfidence = Evaluate(rtt1, rtt2);

    if(ChangeInConfidence == 0 || ChangeInConfidence == -0)
        return;

    //If the evaluation of the rtt and ttl of the endpoint are greater than the
    // largest observed sample so far, make this the new maximum observed sample
    if( ChangeInConfidence > 0 && max_confidence < ChangeInConfidence )
        max_confidence = ChangeInConfidence;
    else if( ChangeInConfidence < 0 && -max_confidence > ChangeInConfidence)
        max_confidence = -ChangeInConfidence;

    location_confidence += ChangeInConfidence;
    if(location_confidence < 0 && location_confidence < -max_confidence)
        location_confidence = -max_confidence;
    else if( location_confidence > 0 && location_confidence > max_confidence)
        location_confidence = max_confidence;

    //If the threshold has been exceeded for either location state, then

    if( location_confidence >= NEAR_THRESHOLD &&
             measurement_location != NEAR_SRC ) {
        measurement_location = NEAR_SRC;
    } else if( location_confidence <= FAR_THRESHOLD &&
             measurement_location != NEAR_DST ) {
        measurement_location = NEAR_DST;
    } else if( (measurement_location == NEAR_SRC && location_confidence < 0.0) ||
             (measurement_location == NEAR_DST && location_confidence > 0.0) ) {
        measurement_location = LOCATION_UNCERTAIN;
    }
    //else, we cannot say that the location is changing decisively in any direction
}

double TCPRS_Analyzer::Evaluate(double rtt1, double rtt2) {
    double confidence = 0.0;
    int orig_diff = GetLikelyTTLDiff(orig->getTTL());
    int resp_diff = GetLikelyTTLDiff(resp->getTTL());
    double ttl_confidence = 0.0;
    double rtt_confidence = 0.0;

    rtt_confidence = rtt2 - rtt1;
    //if this is negative, we are closer to rtt2 than rtt1

    if( rtt1 < rtt2 && rtt2 > 0)
        rtt_confidence /= rtt2;
    else if( rtt1 > rtt2 && rtt1 > 0)
        rtt_confidence /= rtt1;

    //If we have a valid ttl sample, make it bounded by [1.0, 0.0)
    //  else do not modify the current confidence.
    confidence = rtt_confidence;
    if( resp_diff != orig_diff && (orig_diff > 0 || resp_diff > 0) ) {
        if( orig_diff < resp_diff )
            ttl_confidence = (resp_diff - orig_diff) / resp_diff;
        else
            ttl_confidence = (orig_diff - resp_diff) / orig_diff;
        // If the two endpoints aren't "equally spaced apart", modify the confidence
        if( ttl_confidence != 0)
            confidence *= ttl_confidence;
    }

    return confidence;
}

