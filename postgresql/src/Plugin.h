
#ifndef BRO_PLUGIN_BRO_POSTGRES
#define BRO_PLUGIN_BRO_POSTGRES

#include <plugin/Plugin.h>

namespace plugin {
namespace Bro_PostgreSQL {

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
