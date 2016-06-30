##! Packet source using Myricom

module Myricom;

export {
	## Number of rings.
	const snf_num_rings = 1 &redef;
	
	## Size of the ring-buffer in MB.
	const snf_ring_size = 1024 &redef;

	## App ID for the defined rings
    	const snf_app_id = 21 &redef;

	## Define if you would like to aggregate ports.
	## This changes how you define the interface and you must 
	## make the interface a bitmask of all of the Myricom ports
	## in your system.  i.e. "myricom::3" would aggregate ports
	## 0 and 1 in a system with two Myricom ports.
	## Alternately, you can sniff all Myricom interfaces
	## on a system with the special designator "*".  i.e.
	## "myricom::*" would open and aggregate all Myricom interfaces.
	const snf_aggregate = F &redef;

	## Fields available to hash for packet load balancing.
	type RssField: enum {
		SNF_RSS_IP        =  0x01, #<< Include IP (v4 or v6) SRC/DST addr in hash
		SNF_RSS_SRC_PORT  =  0x10, #<< Include TCP/UDP/SCTP SRC port in hash
		SNF_RSS_DST_PORT  =  0x20, #<< Include TCP/UDP/SCTP DST port in hash 
		SNF_RSS_GTP       =  0x40, #<< Include GTP TEID in hash
		SNF_RSS_GRE       =  0x80, #<< Include GRE contents in hash
	};

	## Fields to use in creating the SNF RSS hash for balancing packets
	## out to different Bro processes.
	const snf_rss_mode: set[RssField] = {
		SNF_RSS_IP, 
		SNF_RSS_SRC_PORT, 
		SNF_RSS_DST_PORT
	} &redef;
}
