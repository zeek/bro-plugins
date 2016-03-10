@load ./tcp_retransmissions
@load ./tcp_reordering
@load ./tcp_deadconn
@load ./tcp_options
@load ./tcp_recovery
@load ./tcp_rtt
@load ./tcp_osfp

event bro_init() {
	TCPRS::EnableTCPRSAnalyzer();
}
