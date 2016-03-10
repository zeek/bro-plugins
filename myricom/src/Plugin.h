#ifndef BRO_PLUGIN_BRO_MYRICOM
#define BRO_PLUGIN_BRO_MYRICOM

#include <plugin/Plugin.h>

namespace plugin {
namespace Bro_Myricom {

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
