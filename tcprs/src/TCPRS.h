// See the file "COPYING" in the main distribution directory for copyright.

#ifndef AUX_PLUGINS_TCPRS_TCPRS_H
#define AUX_PLUGINS_TCPRS_TCPRS_H

#include "analyzer/protocol/tcp/TCP.h"
#include "tcprs.bif.h"
#include "tcprs_const.bif.h"


namespace analyzer { namespace tcp {

enum LocationType { NO_LOCATION_DATA, NEAR_SRC, NEAR_DST, LOCATION_UNCERTAIN,
            LOCATION_CONFUSED};

class TCPRS_Endpoint;

class TCPRS_Analyzer : public TCP_ApplicationAnalyzer {
public:
	TCPRS_Analyzer(Connection* conn);
	virtual ~TCPRS_Analyzer();

	virtual void Init();
	virtual void Done();

	void EstimateMeasurementLocation();

	static analyzer::Analyzer* Instantiate(Connection* conn)
		{ return new TCPRS_Analyzer(conn); }

	static bool Available()	{ return true; } // TODO:
	bool TSOptionEnabled () { return UsesTSOption; }
	void sackUsed(bool val) { sack_in_use = val; }

	void IncrementSpurious() { spurious_rexmit_count++; }

protected:

	friend class TCPRS_Endpoint;
        
	void ConfigSummary();

	// 1. process window events
	// 2. call ProcessACK() if appropriate
	// 3. process out-of-order events
	// 4. check for outstanding data
	// 5. insert packet into dict if it contains data
	virtual void DeliverPacket(int len, const u_char* data, bool is_orig,
                                uint64 seq, const IP_Hdr* ip, int caplen);
	
	int GetLikelyDefaultTTL(int ttl);
	int GetLikelyTTLDiff(int ttl);

	void SetInitialRTT(double rtt) { initial_rtt = rtt; }
	double InitialRTT() { return initial_rtt; }

	double initial_rtt;

	double syn_time; 	// time we saw the *last* syn
	double syn_ack_time_1; 	// time we saw the *first* syn-ack
	double syn_ack_time_2; 	// time we saw the *last* syn-ack
	double ack_time; 	// time we saw the *first* ack

	// if there are no rtx's, syn_time will be the time we see the
	// syn, syn_ack_time_1 will equal syn_ack_time_2, and will both be the
	// time we saw the syn-ack, and ack_time will be the time we saw the ack
	LocationType measurement_location;
	double location_confidence;
	double max_confidence;
	uint32 sample_count;

	bool UsesTSOption;

private:
	double Evaluate(double rtt1, double rtt2);
	TCPRS_Endpoint* orig;
	TCPRS_Endpoint* resp;

	bool sack_in_use;
	uint32 spurious_rexmit_count;
};

} } // namespace analyzer::* 

#endif

