
#ifndef BRO_PLUGIN_BRO_MAXMIND
#define BRO_PLUGIN_BRO_MAXMIND

#include <plugin/Plugin.h>

namespace plugin {
namespace Bro_MaxMind {

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
