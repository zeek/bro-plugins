
#include <assert.h>

#include "config.h"

#include "Netmap.h"

using namespace iosource::pktsrc;

NetmapSource::~NetmapSource()
	{
	Close();
	}

NetmapSource::NetmapSource(const std::string& path, bool is_live, const std::string& arg_kind)
	{
	if ( ! is_live )
		Error("netmap source does not support offline input");

	kind = arg_kind;
	current_filter = -1;
	props.path = path;
	props.is_live = is_live;
	last_data = 0;
	}

void NetmapSource::Open()
	{
	std::string iface = kind + ":" + props.path;

	char* rid = getenv("NETMAP_RING_ID");

	if ( rid && *rid )
		iface += "-" + string(rid);

	nd = nm_open(iface.c_str(), 0, 0, 0);

	if ( ! nd )
		{
		Error(errno ? strerror(errno) : "invalid interface");
		return;
		}

	props.netmask = 0xffffff00;
	props.selectable_fd = NETMAP_FD(nd);
	props.is_live = true;
	props.link_type = DLT_EN10MB; // XXX?

	num_discarded = 0;

	Opened(props);
	}

void NetmapSource::Close()
	{
	if ( ! nd )
		return;

	nm_close(nd);
	nd = 0;
	last_data = 0;

	Closed();
	}

bool NetmapSource::ExtractNextPacket(Packet* pkt)
	{
	if ( ! nd )
		return false;

	struct nm_pkthdr hdr;
	const u_char* data;

	while ( true )
		{
		data = nm_nextpkt(nd, &hdr);

		if ( ! data )
			// Source has gone dry.
			return false;

		current_hdr.ts = hdr.ts;
		current_hdr.caplen = hdr.caplen;
		current_hdr.len = hdr.len;

		last_data = data;
		pkt->Init(props.link_type, &current_hdr.ts, current_hdr.caplen, current_hdr.len, data);

		if ( current_hdr.len == 0 || current_hdr.caplen == 0 )
			{
			Weird("empty_netmap_header", pkt);
			return false;
			}

		if ( ApplyBPFFilter(current_filter, &current_hdr, data) )
			// We want this packet.
			break;

		++num_discarded;
		}

	last_hdr = current_hdr;
	last_data = data;
	++stats.received;
	return true;
	}

void NetmapSource::DoneWithPacket()
	{
	// Nothing to do.
	}

bool NetmapSource::PrecompileFilter(int index, const std::string& filter)
	{
	return PktSrc::PrecompileBPFFilter(index, filter);
	}

bool NetmapSource::SetFilter(int index)
	{
	current_filter = index;
	return true;
	}

void NetmapSource::Statistics(Stats* s)
	{
	if ( ! nd )
		{
		s->received = s->link = s->dropped = 0;
		return;
		}

	s->received = stats.received;
	s->link = stats.received + num_discarded;

	// TODO: Seems these counters aren't actually set?
	s->dropped = nd->st.ps_drop + nd->st.ps_ifdrop;
	}

iosource::PktSrc* NetmapSource::InstantiateNetmap(const std::string& path, bool is_live)
	{
	return new NetmapSource(path, is_live, "netmap");
	}

iosource::PktSrc* NetmapSource::InstantiateVale(const std::string& path, bool is_live)
	{
	return new NetmapSource(path, is_live, "vale");
	}
