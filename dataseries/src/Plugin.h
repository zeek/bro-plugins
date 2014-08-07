
#ifndef BRO_PLUGIN_BRO_DATASERIES
#define BRO_PLUGIN_BRO_DATASERIES

#include <plugin/Plugin.h>

namespace plugin {
namespace Bro_DataSeries {

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
