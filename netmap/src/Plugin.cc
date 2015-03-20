
#include "Plugin.h"
#include "Netmap.h"

namespace plugin { namespace Bro_Netmap { Plugin plugin; } }

using namespace plugin::Bro_Netmap;

plugin::Configuration Plugin::Configure()
	{
	AddComponent(new ::iosource::PktSrcComponent("NetmapReader", "netmap", ::iosource::PktSrcComponent::LIVE, ::iosource::pktsrc::NetmapSource::InstantiateNetmap));
	AddComponent(new ::iosource::PktSrcComponent("NetmapReader", "vale", ::iosource::PktSrcComponent::LIVE, ::iosource::pktsrc::NetmapSource::InstantiateVale));

	plugin::Configuration config;
	config.name = "Bro::Netmap";
	config.description = "Packet acquisition via netmap";
	config.version.major = 1;
	config.version.minor = 0;
	return config;
	}
