// See the file "COPYING" in the main distribution directory for copyright.

#ifndef IOSOURCE_PKTSRC_RX_RING_H
#define IOSOURCE_PKTSRC_RX_RING_H

extern "C" {
#include <linux/if_packet.h> // AF_PACKET, etc.
}
#include <cstdint>
#include <stdexcept>

#define TPACKET_VERSION TPACKET_V3

class RX_RingException : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

class RX_Ring {
public:
	/**
	 * Constructor
	 */
	RX_Ring(int sock, size_t bufsize);
	~RX_Ring();

	bool GetNextPacket(tpacket3_hdr** hdr);
	void ReleasePacket(); // Maybe this one should be called by the method ExtractNextPacket before receiving in case DoneWithPacket does not work as expected.

protected:
	void InitLayout(size_t bufsize, size_t snaplen); //TODO: implement snaplen
	void NextBlock();

private:
	struct tpacket_req3 layout;
	struct tpacket_block_desc** blocks;
	struct tpacket3_hdr* packet;

	unsigned int block_num;
	unsigned int packet_num;

	uint8_t* ring;
	size_t size;
};

#endif
