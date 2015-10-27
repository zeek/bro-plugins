
#ifndef BRO_PLUGIN_BRO_TCPRS
#define BRO_PLUGIN_BRO_TCPRS

#include <plugin/Plugin.h>
#include "tcprs_const.bif.h"
#include "tcprs.bif.h"

namespace plugin {
namespace Bro_TCPRS {

bool EnableTCPRS();

class Plugin : public ::plugin::Plugin
{
protected:
	// Overridden from plugin::Plugin.
	virtual plugin::Configuration Configure();
	virtual void HookSetupAnalyzerTree(Connection *conn);
};

extern Plugin plugin;

}
}

#endif
