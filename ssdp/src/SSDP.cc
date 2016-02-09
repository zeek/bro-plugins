#include "SSDP.h"
#include "Reporter.h"
#include "events.bif.h"

using namespace analyzer::SSDP;

SSDP_Analyzer::SSDP_Analyzer(Connection* c)

: analyzer::Analyzer("SSDP", c)
	{
	interp = new binpac::SSDP::SSDP_Conn(this);
	}

SSDP_Analyzer::~SSDP_Analyzer()
	{
	delete interp;
	}

void SSDP_Analyzer::Done()
	{
	Analyzer::Done();
	}

void SSDP_Analyzer::DeliverPacket(int len, const u_char* data,
	 			  bool orig, uint64 seq, const IP_Hdr* ip, int caplen)
	{
	Analyzer::DeliverPacket(len, data, orig, seq, ip, caplen);

	try
		{
		interp->NewData(orig, data, data + len);
		}
	catch ( const binpac::Exception& e )
		{
		ProtocolViolation(fmt("Binpac exception: %s", e.c_msg()));
		}
	}
