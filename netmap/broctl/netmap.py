
import BroControl.plugin
from BroControl import config

class Netmap(BroControl.plugin.Plugin):
    def __init__(self):
        super(Netmap, self).__init__(apiversion=1)

    def name(self):
        return "netmap"

    def pluginVersion(self):
        return 1

    def init(self):
        useplugin = False

        i=0
        for nn in self.nodes():
            if nn.type != "worker" or not nn.interface.startswith("netmap::"):
                continue

            useplugin = True

            nn.interface="{:s}}}{:d}".format(nn.interface, i)
            i = i+1

        return useplugin

    def broctl_config(self):
        script = "# Netmap plugin stuff..."
        #script += "\nredef Myricom::snf_ring_size = %d;" % self.getOption("snf_ring_size")

        #for wn in self.nodes():
        #    if wn.type == "worker": 
        #        if wn.lb_procs > 0:
        #            script += "\n@if( peer_description == \"%s\" )" % wn.name
        #            script += "\n  redef Myricom::snf_num_rings = %s;" % wn.lb_procs
        #            if wn.myricom_snf_ring_size:
        #                script += "\n  redef Myricom::snf_ring_size = %s;" % wn.myricom_snf_ring_size
        #            script += "\n@endif"

        return script

