import BroControl.plugin
import BroControl.config

class Myricom(BroControl.plugin.Plugin):
    def __init__(self):
        super(Myricom, self).__init__(apiversion=1)

    def name(self):
        return "myricom"

    def pluginVersion(self):
        return 1

    def init(self):
        useplugin = False

        interface_pipes = {}
        for nn in self.nodes():
            # Only do this if it's a worker, starts with myricom:: and has some
            # lb_procs configured.
            if nn.type != "worker" or not nn.interface.startswith("myricom::") or not nn.lb_procs:
                continue

            useplugin = True

            orig_if = nn.interface
            if (nn.host, orig_if) not in interface_pipes:
                i = 0
                if nn.myricom_first_ring:
                    i = int(nn.myricom_first_ring)
                interface_pipes[nn.host, orig_if] = i

            nn.interface = "%s:%d" % (orig_if, interface_pipes[nn.host, orig_if])
            interface_pipes[nn.host, orig_if] += 1

        return useplugin

    def nodeKeys(self):
        return ["snf_ring_size", "first_ring"]

    def options(self):
        return [("snf_ring_size", "int", 1024, "Size of host memory buffer")]

    def broctl_config(self):
        script = "# This is the global setting, but can be overridden with per-node configuration"
        script += "\nredef Myricom::snf_ring_size = %d;" % self.getOption("snf_ring_size")

        for wn in self.nodes():
            if wn.type != "worker" or not wn.lb_procs:
                continue

            script += "\n@if( peer_description == \"%s\" )" % wn.name
            script += "\n  redef Myricom::snf_num_rings = %s;" % wn.lb_procs
            if wn.myricom_snf_ring_size:
                script += "\n  redef Myricom::snf_ring_size = %s;" % wn.myricom_snf_ring_size
            script += "\n@endif"

        return script

