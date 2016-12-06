%include binpac.pac
%include bro.pac

%extern{
	#include "events.bif.h"
%}

analyzer SSDP withcontext {
	connection: SSDP_Conn;
	flow:       SSDP_Flow;
};

connection SSDP_Conn(bro_analyzer: BroAnalyzer) {
	upflow   = SSDP_Flow(true);
	downflow = SSDP_Flow(false);
};

%include ssdp-protocol.pac

flow SSDP_Flow(is_orig: bool) {
	 datagram = SSDP_PDU(is_orig) withcontext(connection, this);
};

%include ssdp-analyzer.pac
