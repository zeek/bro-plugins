// See the file  in the main distribution directory for copyright.

#include "Plugin.h"
#include "DataSeries.h"

namespace plugin { namespace Bro_DataSeries { Plugin plugin; } }

using namespace plugin::Bro_DataSeries;

plugin::Configuration Plugin::Configure()
	{
	AddComponent(new ::logging::Component("DataSeries", ::logging::writer::DataSeries::Instantiate));

	plugin::Configuration config;
	config.name = "Bro::DataSeries";
	config.description = "DataSeries log writer";
	config.version.major=1;
	config.version.minor=0;
	return config;
	}
