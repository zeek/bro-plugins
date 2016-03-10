#ifndef BRO_PLUGIN_BRO_KAFKA
#define BRO_PLUGIN_BRO_KAFKA

#include <plugin/Plugin.h>

namespace plugin { namespace Bro_Kafka {

    class Plugin : public ::plugin::Plugin {
    protected:
        // Overridden from plugin::Plugin.
        virtual plugin::Configuration Configure();
    };

    extern Plugin plugin;
}}

#endif
