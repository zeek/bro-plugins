import BroControl.plugin

class AF_Packet(BroControl.plugin.Plugin):
	def __init__(self):
		super(AF_Packet, self).__init__(apiversion=1)

	def name(self):
		return "af_packet"

	def pluginVersion(self):
		return 1

	def init(self):
		# Only use the plugin if there is a worker using AF_PACKET for load balancing.
		for nn in self.nodes():
			if nn.type == "worker" and nn.interface.startswith("af_packet::") and nn.lb_procs:
				return True

		return False

	def nodeKeys(self):
		return ["fanout_id", "fanout_mode"]

	def broctl_config(self):
		script = ""

		# Add custom configuration values per worker.
		for nn in self.nodes():
			if nn.type != "worker" or not nn.lb_procs:
				continue

			if nn.af_packet_fanout_id or nn.af_packet_fanout_mode:
				script += "\n@if( peer_description == \"%s\" )" % nn.name
				if nn.af_packet_fanout_id:
					script += "\n  redef AF_Packet::fanout_id = %s;" % nn.af_packet_fanout_id
				if nn.af_packet_fanout_mode:
					script += "\n  redef AF_Packet::fanout_mode = %s;" % nn.af_packet_fanout_mode
				script += "\n@endif"

		return script
