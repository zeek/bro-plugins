
#include "Plugin.h"

namespace plugin { namespace Bro_MaxMind { Plugin plugin; } }

using namespace plugin::Bro_MaxMind;

plugin::Configuration Plugin::Configure()
	{
	plugin::Configuration config;
	config.name = "Bro::MaxMind";
	config.description = "Bro support for the MaxMindDB library.";
	config.version.major = 0;
	config.version.minor = 1;
	return config;
	}
