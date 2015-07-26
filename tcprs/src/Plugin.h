
#ifndef BRO_PLUGIN_JSWARO_TCPRS
#define BRO_PLUGIN_JSWARO_TCPRS

#include <plugin/Plugin.h>
#include "tcprs_const.bif.h"
#include "tcprs.bif.h"

namespace plugin {
namespace jswaro_tcprs {

class Plugin : public ::plugin::Plugin
{
protected:
	// Overridden from plugin::Plugin.
	virtual plugin::Configuration Configure();
	virtual void HookAddToAnalyzerTree(Connection *conn);
};

extern Plugin plugin;

}
}

#endif
