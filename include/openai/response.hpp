#pragma once
#include <nlohmann/json.hpp>

namespace openai {

struct ResponseRequest {};

void to_json(nlohmann::json &j, const ResponseRequest &req);
void from_json(const nlohmann::json &j, ResponseRequest &req);

struct Response {};

void to_json(nlohmann::json &j, const Response &resp);
void from_json(const nlohmann::json &j, Response &resp);

} // namespace openai
