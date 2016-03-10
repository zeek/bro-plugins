#ifndef BRO_PLUGIN_BRO_KAFKA_TAGGEDJSON_H
#define BRO_PLUGIN_BRO_KAFKA_TAGGEDJSON_H

#include <string>
#include <threading/Formatter.h>
#include <threading/formatters/JSON.h>

using threading::formatter::JSON;
using threading::MsgThread;
using threading::Value;
using threading::Field;

namespace threading { namespace formatter {

/*
 * A JSON formatter that prepends or 'tags' the content with a log stream
 * identifier.  For example,
 *   { 'conn' : { ... }}
 *   { 'http' : { ... }}
 */
class TaggedJSON : public JSON {

public:
    TaggedJSON(string stream_name, MsgThread* t, JSON::TimeFormat tf);
    virtual ~TaggedJSON();
    virtual bool Describe(ODesc* desc, int num_fields, const Field* const* fields, Value** vals) const;

private:
    string stream_name;
};

}}
#endif
