
#ifndef BRO_PLUGIN_BRO_REDIS
#define BRO_PLUGIN_BRO_REDIS

#include <plugin/Plugin.h>

namespace plugin {
namespace Bro_RedisWriter {

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
