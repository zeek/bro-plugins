#include "TaggedJSON.h"

namespace threading { namespace formatter {

TaggedJSON::TaggedJSON(string sn, MsgThread* t, JSON::TimeFormat tf): JSON(t, tf), stream_name(sn)
{}

TaggedJSON::~TaggedJSON()
{}

bool TaggedJSON::Describe(ODesc* desc, int num_fields, const Field* const* fields, Value** vals) const
{
    desc->AddRaw("{");

    // 'tag' the json; aka prepend the stream name to the json-formatted log content
    desc->AddRaw("\"");
    desc->AddRaw(stream_name);
    desc->AddRaw("\": ");

    // append the JSON formatted log record itself
    JSON::Describe(desc, num_fields, fields, vals);

    desc->AddRaw("}");
    return true;
}
}}
