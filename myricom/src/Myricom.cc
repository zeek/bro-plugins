#include "bro-config.h"

#include "Myricom.h"
#include "myricom.bif.h"

using namespace iosource::pktsrc;

MyricomSource::~MyricomSource()
	{
	Close();
	}

MyricomSource::MyricomSource(const std::string& path, bool is_live, const std::string& arg_kind)
	{
	if ( ! is_live )
		Error("Myricom source does not support offline input");

	kind = arg_kind;
	current_filter = -1;
	props.path = path;
	props.is_live = is_live;
	}

static inline struct timeval snf_timestamp_to_timeval(const int64_t ts_nanosec)
{
    struct timeval tv;
    long tv_nsec;

    if (ts_nanosec == 0)
        return (struct timeval) { 0, 0 };

    tv.tv_sec = ts_nanosec / _NSEC_PER_SEC;
    tv_nsec = (ts_nanosec % _NSEC_PER_SEC);
    tv.tv_usec = tv_nsec / 1000;

    return tv;
}

void MyricomSource::Open()
	{
    uint64_t snf_num_rings = BifConst::Myricom::snf_num_rings;
    uint64_t snf_ring_size = BifConst::Myricom::snf_ring_size;
    const char * snf_rss =  reinterpret_cast<const char*>(BifConst::Myricom::snf_rss->Bytes());
    const char * snf_flags =  reinterpret_cast<const char*>(BifConst::Myricom::snf_flags->Bytes());
    snf_link_state snf_link_isup;
    snf_timesource_state snf_timesource_active;
    std::string ts_local = "Local timesource (no external)";
    std::string ts_ext_unsynced = "External Timesource: not synchronized (yet)";
    std::string ts_ext_synced = "External Timesource: synchronized";
    std::string ts_ext_failed = "External Timesource: NIC failure to connect to source";
    std::string ts_arista_active = "Arista switch is sending ptp timestamps";
	std::string iface = props.path;
    struct snf_ifaddrs *ifaddrs = NULL, *ifa;
    size_t devlen;
    uint32_t portnum = -1;

    if ( snf_init(SNF_VERSION_API) != 0) {
        Error(errno ? strerror(errno) : "SNF: failed in snf_init");
        return;
    }

    if ( snf_getifaddrs(&ifaddrs) != 0 ) {
        Error(errno ? strerror(errno) : "SNF: failed in snf_getifaddrs");
        return;
    }

    devlen = strlen(iface.data()) + 1;
    ifa = ifaddrs;
    while (ifa != NULL) {
        if (!strncmp(iface.data(), ifa->snf_ifa_name, devlen)) {
            portnum = ifa->snf_ifa_boardnum;
            break;
        }
        ifa = ifa->snf_ifa_next;
    }
    snf_freeifaddrs(ifaddrs);

    if ( ifa == NULL ) {
        Error(errno ? strerror(errno) : "SNF: failed to map the interface to the board numer");
        return;
    }

    if ( snf_open(portnum, snf_num_rings, NULL, snf_ring_size, SNF_F_PSHARED, &snf_handle) != 0 ) {
        Error(errno ? strerror(errno) : "SNF: failed in snf_open");
        return;
    }

    if ( snf_get_link_state(snf_handle, &snf_link_isup) != 0 ) {
        Error(errno ? strerror(errno) : "SNF: failed in snf_get_link_state");
        return;
    }

    if (snf_link_isup != SNF_LINK_UP) {
        Error(errno ? strerror(errno) : "SNF: interface is down");
        return;
    }

    if ( snf_get_timesource_state(snf_handle, &snf_timesource_active) != 0 ) {
        Error(errno ? strerror(errno) : "SNF: failed in snf_get_timesource_state");
        return;
    }

    switch(snf_timesource_active) {
        case SNF_TIMESOURCE_LOCAL:
            printf("SNF: %s\n", ts_local.data());
            break;
        case SNF_TIMESOURCE_EXT_UNSYNCED:
            printf("SNF: %s\n", ts_ext_unsynced.data());
            break;
        case SNF_TIMESOURCE_EXT_SYNCED:
            printf("SNF: %s\n", ts_ext_synced.data());
            break;
        case SNF_TIMESOURCE_EXT_FAILED:
            printf("SNF: %s\n", ts_ext_failed.data());
            break;
        case SNF_TIMESOURCE_ARISTA_ACTIVE:
            printf("SNF: %s\n", ts_arista_active.data());
            break;
        default:
            printf("SNF: I have no idea what the timesource is\n");
    }
    fflush(stdout);


    if ( snf_ring_open(snf_handle, &snf_ring) ) {
        Error(errno ? strerror(errno) : "failed in snf_ring_open");
        return;
    }

    if ( snf_start(snf_handle) ) {
        Error(errno ? strerror(errno) : "failed in snf_start");
        return;
    }

	props.netmask = 0xffffff00;
	props.is_live = true;
	props.link_type = DLT_EN10MB; // XXX?

	num_discarded = 0;

	Opened(props);
	}

void MyricomSource::Close()
	{
	if ( ! snf_ring || ! snf_handle )
		return;
    
    snf_ring_close(snf_ring);
    snf_close(snf_handle);

    snf_handle = NULL;
	snf_ring = NULL;

	Closed();
	}

bool MyricomSource::ExtractNextPacket(Packet* pkt)
	{
    struct snf_recv_req recv_req;
	u_char *data;
	int rc;
    int timeout_ms = 0;

	if ( ! snf_ring )
		return false;

	while ( true )
		{
		if ( snf_ring_recv(snf_ring, timeout_ms, &recv_req) != 0 )
			return false;

		current_hdr.ts = snf_timestamp_to_timeval(recv_req.timestamp);
		current_hdr.caplen = recv_req.length;
		current_hdr.len = recv_req.length;
        data = (unsigned char *) recv_req.pkt_addr;

		pkt->Init(props.link_type, &current_hdr.ts, current_hdr.caplen, current_hdr.len, data);

		if ( ApplyBPFFilter(current_filter, &current_hdr, data) )
			break;

		++num_discarded;
		}

	return true;
	}

void MyricomSource::DoneWithPacket()
	{
        // Nothing to do.
	}

bool MyricomSource::PrecompileFilter(int index, const std::string& filter)
	{
	return PktSrc::PrecompileBPFFilter(index, filter);
	}

bool MyricomSource::SetFilter(int index)
	{
	current_filter = index;
	return true;
	}

void MyricomSource::Statistics(Stats* s)
	{
	snf_ring_stats ps;

	if ( ! snf_ring || snf_ring_getstats(snf_ring, &ps) != 0)
		{
		s->received = s->link = s->dropped = 0;
		return;
		}

	s->received = ps.ring_pkt_recv + ps.ring_pkt_overflow;
	s->link = ps.nic_pkt_recv;
	s->dropped = ps.ring_pkt_overflow;
	}

iosource::PktSrc* MyricomSource::InstantiateMyricom(const std::string& path, bool is_live)
	{
	return new MyricomSource(path, is_live, "myricom");
	}

