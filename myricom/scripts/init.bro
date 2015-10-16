#! Packet source using Myricom

module Myricom;

export {
    ## 
    const snf_num_rings = 16;
    ## Size of the ring-buffer.
    const snf_ring_size = 32 * 1024 * 1024 * 1024 &redef;
    ## not yet supported - use env variables
    const snf_flags = "jakieszabawneflagi" &redef;
    ## not yet supported - use env variables
    const snf_rss = "alamakota|alamadwakoty|alamatrzykoty" &redef;
}
