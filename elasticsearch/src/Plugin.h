
#ifndef BRO_PLUGIN_BRO_ELASTICSEARCH
#define BRO_PLUGIN_BRO_ELASTICSEARCH

#include <plugin/Plugin.h>

namespace plugin {
namespace Bro_ElasticSearch {

class Plugin : public ::plugin::Plugin
{
protected:
	// Overridden from plugin::Plugin.
	virtual plugin::Configuration Configure();
};

extern Plugin plugin;

}
}

#endif
