##! Packet source using Myricom

module Myricom;

export {
    ## Number of rings.
    const snf_num_rings = 16 &redef;
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
}
