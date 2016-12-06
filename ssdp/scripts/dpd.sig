signature dpd_ssdp {
	ip-proto == udp
	payload /^(NOTIFY|M-SEARCH)/
	enable "ssdp"
}
