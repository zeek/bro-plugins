
#include "Plugin.h"
#include "PostgresWriter.h"
#include "PostgresReader.h"

namespace plugin { namespace Bro_PostgreSQL { Plugin plugin; } }

using namespace plugin::Bro_PostgreSQL;

plugin::Configuration Plugin::Configure()
	{
	AddComponent(new ::logging::Component("PostgreSQL", ::logging::writer::PostgreSQL::Instantiate));
	AddComponent(new ::input::Component("PostgreSQL", ::input::reader::PostgreSQL::Instantiate));

	plugin::Configuration config;
	config.name = "Bro::PostgreSQL";
	config.description = "PostgreSQL log writer and input reader";
	config.version.major = 0;
	config.version.minor = 1;
	return config;
	}
