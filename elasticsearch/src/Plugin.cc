// See the file  in the main distribution directory for copyright.

#include "Plugin.h"
#include "ElasticSearch.h"

namespace plugin { namespace Bro_ElasticSearch { Plugin plugin; } }

using namespace plugin::Bro_ElasticSearch;

plugin::Configuration Plugin::Configure()
	{
	AddComponent(new ::logging::Component("ElasticSearch", ::logging::writer::ElasticSearch::Instantiate));

	plugin::Configuration config;
	config.name = "Bro::ElasticSearch";
	config.description = "ElasticSearch log writer";
	config.version.major=1;
	config.version.minor=0;
	return config;
	}

