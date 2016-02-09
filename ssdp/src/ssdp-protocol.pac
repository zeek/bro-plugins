type SSDP_COLON	= RE/:/;
type SSDP_WS	= RE/[ \t]*/;
type SSDP_NAME 	= RE/[^\:]*/;
type SSDP_VALUE	= RE/[^\x0d]*/;
type SSDP_EOL	= RE/(\r\n){1,2}/;

type Header = record {
	name:		SSDP_NAME;
	seperator:	SSDP_COLON;
	space:		SSDP_WS;
	value:		SSDP_VALUE;
	carriage:	SSDP_EOL;
} &oneline;

type Method = record {
        method: 	RE/^(NOTIFY|M-SEARCH)/;
	space:		SSDP_WS;
	uri:		"*";
	space2:		SSDP_WS;
	version:	"HTTP/1.1";
	carriage:	SSDP_EOL;
};

type SSDP_PDU(is_orig: bool) = record {
	method:	Method;
	hdr:	Header[] &until($input.length() == 0);
} &byteorder=bigendian;
