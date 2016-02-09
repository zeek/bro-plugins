#ifndef BRO_PLUGIN_BRO_SSDP
#define BRO_PLUGIN_BRO_SSDP

#include <plugin/Plugin.h>

namespace plugin {
namespace Bro_SSDP {

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
