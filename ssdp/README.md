SSDP Analyzer
=================================

This plugin is an SSDP protocol analyzer for Bro. The analyzer parses the discovery phase of the SSDP protocol. When used for processing SSDP network traffic, this plugin will generate ssdp.log-- the log contains the type of SSDP discovery message (REQUEST or RESPONSE), all headers seen in the message, and metadata relevant to the message (location of the description file, USN, server information, etc). 

Note that SSDP network traffic may represent a significant portion of overall network traffic, so caution is advised if deploying this analyzer in a production environment-- it is possible that this analyzer will generate large ssdp.log files. Profiling overall network traffic by searching conn.log for any UDP connections on port 1900 to get a sense of how much potential traffic will be parsed with this analyzer plugin is recommended. Whitelisting of specific values will reduce the log output of the analyzer (whitelisting functionality doesn't currently exist, but can be added to scripts/main.bro).

Additionally, this plugin will only work as intended with Bro 2.4-214 or later. The protocol analyzer code for this plugin comes from the binpac_quickstart template, available here: https://github.com/grigorescu/binpac_quickstart. 

### Installation

Refer to the documentation on this page for installation instructions: https://www.bro.org/sphinx-git/devel/plugins.html

### Log output
```
#separator \x09
#set_separator	,
#empty_field	(empty)
#unset_field	-
#path	ssdp
#open	2015-12-07-11-54-45
#fields	ts	uid	id.orig_h	id.orig_p	id.resp_h	id.resp_p	method	search_target	server	usn	location	headers
#types	time	string	addr	port	addr	port	string	string	string	string	string	set[string]
1129437657.087179	C8J9gR1Kjinwoa2Ui8	192.168.0.1	1900	239.255.255.250	1900	NOTIFY	urn:schemas-upnp-org:service:WANPPPConnection:1	Microsoft-Windows-NT/5.1 UPnP/1.0 UPnP-Device-Host/1.0	uuid:69d5b057-5ec7-4a37-bfb4-b845d0461c77::urn:schemas-upnp-org:service:WANPPPConnection:1	http://192.168.0.1:2869/upnphost/udhisapi.dll?content=uuid:11a6f064-4791-4477-a680-e0b3ce8c79c3	NT,Cache-Control,NTS,USN,Host,Server,Location
1129437657.106677	C8J9gR1Kjinwoa2Ui8	192.168.0.1	1900	239.255.255.250	1900	NOTIFY	urn:schemas-upnp-org:service:WANPOTSLinkConfig:1	Microsoft-Windows-NT/5.1 UPnP/1.0 UPnP-Device-Host/1.0	uuid:69d5b057-5ec7-4a37-bfb4-b845d0461c77::urn:schemas-upnp-org:service:WANPOTSLinkConfig:1	http://192.168.0.1:2869/upnphost/udhisapi.dll?content=uuid:11a6f064-4791-4477-a680-e0b3ce8c79c3	NT,Cache-Control,NTS,USN,Host,Server,Location
1129437657.126782	C8J9gR1Kjinwoa2Ui8	192.168.0.1	1900	239.255.255.250	1900	NOTIFY	urn:schemas-upnp-org:device:WANConnectionDevice:1	Microsoft-Windows-NT/5.1 UPnP/1.0 UPnP-Device-Host/1.0	uuid:69d5b057-5ec7-4a37-bfb4-b845d0461c77::urn:schemas-upnp-org:device:WANConnectionDevice:1	http://192.168.0.1:2869/upnphost/udhisapi.dll?content=uuid:11a6f064-4791-4477-a680-e0b3ce8c79c3	NT,Cache-Control,NTS,USN,Host,Server,Location
1129437657.130206	C8J9gR1Kjinwoa2Ui8	192.168.0.1	1900	239.255.255.250	1900	NOTIFY	uuid:69d5b057-5ec7-4a37-bfb4-b845d0461c77	Microsoft-Windows-NT/5.1 UPnP/1.0 UPnP-Device-Host/1.0	uuid:69d5b057-5ec7-4a37-bfb4-b845d0461c77	http://192.168.0.1:2869/upnphost/udhisapi.dll?content=uuid:11a6f064-4791-4477-a680-e0b3ce8c79c3	NT,Cache-Control,NTS,USN,Host,Server,Location
1129437657.167605	C8J9gR1Kjinwoa2Ui8	192.168.0.1	1900	239.255.255.250	1900	NOTIFY	urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1	Microsoft-Windows-NT/5.1 UPnP/1.0 UPnP-Device-Host/1.0	uuid:b8824b29-1785-41e9-9c2b-2a341be384ef::urn:schemas-upnp-org:service:WANCommonInterfaceConfig:1	http://192.168.0.1:2869/upnphost/udhisapi.dll?content=uuid:11a6f064-4791-4477-a680-e0b3ce8c79c3	NT,Cache-Control,NTS,USN,Host,Server,Location
1129437657.188954	C8J9gR1Kjinwoa2Ui8	192.168.0.1	1900	239.255.255.250	1900	NOTIFY	urn:schemas-upnp-org:device:WANDevice:1	Microsoft-Windows-NT/5.1 UPnP/1.0 UPnP-Device-Host/1.0	uuid:b8824b29-1785-41e9-9c2b-2a341be384ef::urn:schemas-upnp-org:device:WANDevice:1	http://192.168.0.1:2869/upnphost/udhisapi.dll?content=uuid:11a6f064-4791-4477-a680-e0b3ce8c79c3	NT,Cache-Control,NTS,USN,Host,Server,Location
1129437657.197006	C8J9gR1Kjinwoa2Ui8	192.168.0.1	1900	239.255.255.250	1900	NOTIFY	uuid:b8824b29-1785-41e9-9c2b-2a341be384ef	Microsoft-Windows-NT/5.1 UPnP/1.0 UPnP-Device-Host/1.0	uuid:b8824b29-1785-41e9-9c2b-2a341be384ef	http://192.168.0.1:2869/upnphost/udhisapi.dll?content=uuid:11a6f064-4791-4477-a680-e0b3ce8c79c3	NT,Cache-Control,NTS,USN,Host,Server,Location
```
