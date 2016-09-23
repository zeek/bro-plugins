#ifndef BRO_PLUGIN_BRO_KAFKA_KAFKAWRITER_H
#define BRO_PLUGIN_BRO_KAFKA_KAFKAWRITER_H

#include <map>
#include <string>

#include <logging/WriterBackend.h>

namespace RdKafka {
    class Conf;
    class Producer;
    class Topic;
}

namespace threading {
  namespace formatter {
    class Formatter;
}}

namespace logging { namespace writer {

/**
 * A logging writer that sends data to a Kafka broker.
 */
class KafkaWriter : public WriterBackend {

public:
    KafkaWriter(WriterFrontend* frontend);
    ~KafkaWriter();

    static WriterBackend* Instantiate(WriterFrontend* frontend)
    {
        return new KafkaWriter(frontend);
    }

protected:
    virtual bool DoInit(const WriterBackend::WriterInfo& info, int num_fields, const threading::Field* const* fields);
    virtual bool DoWrite(int num_fields, const threading::Field* const* fields, threading::Value** vals);
    virtual bool DoSetBuf(bool enabled);
    virtual bool DoRotate(const char* rotated_path, double open, double close, bool terminating);
    virtual bool DoFlush(double network_time);
    virtual bool DoFinish(double network_time);
    virtual bool DoHeartbeat(double network_time, double current_time);

private:
    static const string default_topic_key;

    string stream_id;
    string topic_name;
    bool tag_json;
    map<string, string> kafka_conf;

    threading::formatter::Formatter *formatter;

    RdKafka::Producer* rd_producer;
    RdKafka::Topic* rd_topic;
    RdKafka::Conf* rd_conf;
    RdKafka::Conf* rd_topic_conf;
};

}}

#endif
