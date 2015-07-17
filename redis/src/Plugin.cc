// See the file  in the main distribution directory for copyright.

#include "Plugin.h"
#include "Redis.h"

namespace plugin { namespace Bro_RedisWriter { Plugin plugin; } }

using namespace plugin::Bro_RedisWriter;

plugin::Configuration Plugin::Configure()
	{
	AddComponent(new ::logging::Component("Redis", ::logging::writer::Redis::Instantiate));

	plugin::Configuration config;
	config.name = "Bro::Redis";
	config.description = "Redis log writer";
	config.version.major = 1;
	config.version.minor = 0;
	return config;
	}
