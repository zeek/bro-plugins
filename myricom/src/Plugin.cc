#include "Plugin.h"
#include "Myricom.h"

namespace plugin { namespace Bro_Myricom { Plugin plugin; } }

using namespace plugin::Bro_Myricom;

plugin::Configuration Plugin::Configure()
{
    AddComponent(new ::iosource::PktSrcComponent("MyricomReader", "myricom", ::iosource::PktSrcComponent::LIVE, ::iosource::pktsrc::MyricomSource::InstantiateMyricom));

    plugin::Configuration config;
    config.name = "Bro::Myricom";
    config.description = "Packet acquisition via Myricom SNF v3+v4";
    config.version.major = 1;
    config.version.minor = 0;

    return config;
}
