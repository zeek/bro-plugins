##! Packet source using Myricom

module Myricom;

export {
    ## Number of rings.
    const snf_num_rings = 16 &redef;
    ## Size of the ring-buffer in MB.
    const snf_ring_size = 1024 &redef;
    ## App ID for the defined rings
    const snf_app_id = 21 &redef;
}
