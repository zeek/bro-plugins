#ifndef ANALYZER_PROTOCOL_SSDP_SSDP_H
#define ANALYZER_PROTOCOL_SSDP_SSDP_H
#include "events.bif.h"
#include "analyzer/protocol/udp/UDP.h"
#include "ssdp_pac.h"

namespace analyzer { namespace SSDP {

class SSDP_Analyzer

: public analyzer::Analyzer {

public:
	SSDP_Analyzer(Connection* conn);
	virtual ~SSDP_Analyzer();

	// Overriden from Analyzer.
	virtual void Done();
	
	virtual void DeliverPacket(int len, const u_char* data, bool orig,
					uint64 seq, const IP_Hdr* ip, int caplen);
	

	static analyzer::Analyzer* Instantiate(Connection* conn)
		{ return new SSDP_Analyzer(conn); }

protected:
	binpac::SSDP::SSDP_Conn* interp;
	
};

} } // namespace analyzer::* 

#endif
