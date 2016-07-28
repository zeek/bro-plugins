module Kafka;

export {
  const topic_name: string = "" &redef;
  const max_wait_on_shutdown: count = 3000 &redef;
  const tag_json: bool = F &redef;
  const kafka_conf: table[string] of string = table(
    ["metadata.broker.list"] = "localhost:9092"
  ) &redef;
}
