
#ifndef BRO_PLUGIN_BRO_NETMAP
#define BRO_PLUGIN_BRO_NETMAP

#include <plugin/Plugin.h>

namespace plugin {
namespace Bro_Netmap {

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
