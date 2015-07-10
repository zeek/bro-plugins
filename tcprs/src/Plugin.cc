#include "Plugin.h"
#include "TCPRS.h"

namespace plugin { namespace jswaro_tcprs { Plugin plugin; } }

using namespace plugin::jswaro_tcprs;

static bool use_tcprs_analyzer = false;

plugin::Configuration Plugin::Configure()
	{
	AddComponent(new ::analyzer::Component("TCPRS", ::analyzer::tcp::TCPRS_Analyzer::Instantiate));

	plugin::Configuration config;
	config.name = "jswaro::tcprs";
	config.description = "A TCP retransmission and network reordering detection and classification analyzer";
	config.version.major = 1;
	config.version.minor = 0;


	EnableHook(HOOK_ADD_TO_ANALYZER_TREE, 0);

	return config;
	}

void Plugin::HookAddToAnalyzerTree(Connection *conn)
	{
		analyzer::tcp::TCP_Analyzer* tcp = 0;
		analyzer::TransportLayerAnalyzer* root = 0;

		if ( conn->ConnTransport() != TRANSPORT_TCP )
			return;

		if ( !use_tcprs_analyzer )
			return;

		root = conn->GetRootAnalyzer();
		tcp = (analyzer::tcp::TCP_Analyzer *) root;

		analyzer::tcp::TCPRS_Analyzer* tcprs = new analyzer::tcp::TCPRS_Analyzer(conn);

		tcp->AddChildPacketAnalyzer(tcprs);

		/* init the new analyzer */
		tcprs->Init();
	}

bool plugin::jswaro_tcprs::EnableTCPRS()
	{
		use_tcprs_analyzer = true;
		return use_tcprs_analyzer;
	}
