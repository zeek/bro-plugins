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

        interface_pipes={}
        for nn in self.nodes():
            # Only do this if it's a worker, starts with netmap:: and has some lb_proc configured.
            if nn.type != "worker" or not nn.interface.startswith("netmap::") or nn.lb_procs == "":
                continue

            useplugin = True

            orig_if = nn.interface
            if (orig_if,nn.host) not in interface_pipes:
                i=0
                if nn.netmap_first_pipe:
                    i = int(nn.netmap_first_pipe)
                interface_pipes[nn.host,orig_if] = i

            nn.interface="{:s}}}{:d}".format(orig_if, interface_pipes[nn.host,orig_if])
            interface_pipes[nn.host,orig_if] = interface_pipes[nn.host,orig_if]+1

        return useplugin

    def nodeKeys(self):
        return ["first_pipe"]

    def broctl_config(self):
        pass
