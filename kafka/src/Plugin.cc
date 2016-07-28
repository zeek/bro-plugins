#include "Plugin.h"
#include "KafkaWriter.h"

namespace plugin { namespace Bro_Kafka {
    Plugin plugin;
}}

using namespace plugin::Bro_Kafka;

plugin::Configuration Plugin::Configure()
{
    AddComponent(new ::logging::Component("KafkaWriter", ::logging::writer::KafkaWriter::Instantiate));

    plugin::Configuration config;
    config.name = "Bro::Kafka";
    config.description = "Writes logs to Kafka";
    config.version.major = 0;
    config.version.minor = 1;
    return config;
}
