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

    int colon = path.find(":");
    if ( colon > 0 ) {
        iface = path.substr(0,colon).c_str();
        ring_num = atoi(path.substr(colon+1,-1).c_str());
    } else {
        // -1 is a special number that makes it just open any available ring.
        ring_num = -1;
    }

    props.path = iface;
    props.is_live = is_live;
}

static inline struct timeval snf_timestamp_to_timeval(const int64_t ts_nanosec)
{
    struct timeval tv;
    long tv_nsec;

    if (ts_nanosec == 0) {
        return (struct timeval) { 0, 0 };
    } else {
        tv.tv_sec = ts_nanosec / _NSEC_PER_SEC;
        tv_nsec = (ts_nanosec % _NSEC_PER_SEC);
        tv.tv_usec = tv_nsec / 1000;
    }

    return tv;
}

void MyricomSource::Open()
{
    uint64_t snf_num_rings = BifConst::Myricom::snf_num_rings;
    uint64_t snf_ring_size = BifConst::Myricom::snf_ring_size;
    uint64_t snf_app_id = BifConst::Myricom::snf_app_id;
    snf_link_state snf_link_isup;
    snf_timesource_state snf_timesource_active;
    std::string ts_local = "Local timesource (no external)";
    std::string ts_ext_unsynced = "External Timesource: not synchronized (yet)";
    std::string ts_ext_synced = "External Timesource: synchronized";
    std::string ts_ext_failed = "External Timesource: NIC failure to connect to source";
    std::string ts_arista_active = "Arista switch is sending ptp timestamps";
    struct snf_ifaddrs *ifaddrs = NULL, *ifa;
    uint32_t portnum = -1;

    if ( snf_init(SNF_VERSION_API) != 0) {
        Error(errno ? strerror(errno) : "SNF: failed in snf_init");
        return;
    }

    if ( snf_getifaddrs(&ifaddrs) != 0 ) {
        Error(errno ? strerror(errno) : "SNF: failed in snf_getifaddrs");
        return;
    }

    if ( snf_set_app_id(snf_app_id & 0xFFFFFFFF) ) {
        Error(errno ? strerror(errno) : "SNF: failed in snf_set_app_id");
        return;
    }

    if ( snf_getifaddrs(&ifaddrs) ) {
        Error(errno ? strerror(errno) : "SNF: failed in snf_getifaddrs");
        return;
    }
    ifa = ifaddrs;
    while (ifa != NULL) {
        if (!strncmp(iface.data(), ifa->snf_ifa_name, iface.length())) {
            portnum = ifa->snf_ifa_boardnum;
            break;
        }
        ifa = ifa->snf_ifa_next;
    }
    snf_freeifaddrs(ifaddrs);

    if ( ifa == NULL ) {
        Error(errno ? strerror(errno) : "SNF: failed to map the interface to the board number");
        return;
    }

    struct snf_rss_params rssp;
    rssp.mode = SNF_RSS_FLAGS;
    rssp.params.rss_flags = (snf_rss_mode_flags) (SNF_RSS_IP | SNF_RSS_SRC_PORT | SNF_RSS_DST_PORT);

printf("opening port:  %d  - num rings: %lu - ring num: %d - ring size %lu\n", portnum, snf_num_rings, ring_num, snf_ring_size);

    if ( snf_open(portnum, snf_num_rings, &rssp, snf_ring_size, SNF_F_PSHARED, &snf_handle) != 0 ) {
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


    if ( snf_ring_open_id(snf_handle, ring_num, &snf_ring) ) {
        Error(errno ? strerror(errno) : "failed in snf_ring_open_id");
        return;
    }

    if ( snf_start(snf_handle) ) {
        Error(errno ? strerror(errno) : "failed in snf_start");
        return;
    }

    props.netmask = NETMASK_UNKNOWN;
    props.is_live = true;
    props.link_type = DLT_EN10MB;

    num_received = 0;
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

    if ( ! snf_ring )
        return false;

    while ( true ) {
        if ( snf_ring_recv(snf_ring, 1, &recv_req) != 0 )
            // No packets right now.
            return false;

        if ( recv_req.timestamp == 0.0 )
            {
            // TODO: If a packet source returns zero, Bro starts ignoring the packet source.
            Error("Myricom packet source returned a zero timestamp!");
            return false;
            }

        current_hdr.ts = snf_timestamp_to_timeval(recv_req.timestamp);
        current_hdr.caplen = recv_req.length;
        current_hdr.len = recv_req.length;
        data = (unsigned char *) recv_req.pkt_addr;

        if ( !ApplyBPFFilter(current_filter, &current_hdr, data) ) {
            ++num_discarded;
            continue;
        }

        pkt->Init(props.link_type, &current_hdr.ts, current_hdr.caplen, current_hdr.len, data);
        ++num_received;
        return true;
    }

    // Shouldn't be able to reach this point...
    return false;
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

    if ( ! snf_ring || snf_ring_getstats(snf_ring, &ps) != 0) {
        s->received = s->link = s->dropped = 0;
        return;
    }

    s->received = num_received;
    s->link = ps.ring_pkt_recv;
    s->dropped = ps.ring_pkt_overflow;
}

iosource::PktSrc* MyricomSource::InstantiateMyricom(const std::string& path, bool is_live)
{
    return new MyricomSource(path, is_live, "myricom");
}

