
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

            if i == 0 and nn.netmap_first_pipe:
                i = int(nn.netmap_first_pipe)

            nn.interface="{:s}}}{:d}".format(nn.interface, i)
            i = i+1

        return useplugin

    def nodeKeys(self):
        return ["first_pipe"]

    def broctl_config(self):
        pass

