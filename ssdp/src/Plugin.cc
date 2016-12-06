#include "plugin/Plugin.h"
#include "SSDP.h"

namespace plugin {
namespace Bro_SSDP {

class Plugin : public plugin::Plugin {
public:
	plugin::Configuration Configure()
		{
		AddComponent(new ::analyzer::Component("SSDP", ::analyzer::SSDP::SSDP_Analyzer::Instantiate));

		plugin::Configuration config;
		config.name = "Bro::SSDP";
		config.description = "SSDP analyzer";
		config.version.major = 0;
		config.version.minor = 2;
		return config;
		}
} plugin;

}
}
