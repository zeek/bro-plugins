import BroControl.plugin
import BroControl.config

class Netmap(BroControl.plugin.Plugin):
    def __init__(self):
        super(Netmap, self).__init__(apiversion=1)

    def name(self):
        return "netmap"

    def pluginVersion(self):
        return 1

    def init(self):
        useplugin = False

        interface_pipes = {}
        for nn in self.nodes():
            # Only do this if it's a worker, starts with netmap:: and has some
            # lb_procs configured.
            if nn.type != "worker" or not nn.interface.startswith("netmap::") or not nn.lb_procs:
                continue

            useplugin = True

            orig_if = nn.interface
            if (nn.host, orig_if) not in interface_pipes:
                i = 0
                if nn.netmap_first_pipe:
                    i = int(nn.netmap_first_pipe)
                interface_pipes[nn.host, orig_if] = i

            nn.interface="%s}%d" % (orig_if, interface_pipes[nn.host, orig_if])
            interface_pipes[nn.host, orig_if] += 1

        return useplugin

    def nodeKeys(self):
        return ["first_pipe"]

    def broctl_config(self):
        pass
