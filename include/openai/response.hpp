#pragma once
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <variant>

namespace openai {

struct ResponseRequest {
  std::optional<nlohmann::json> input;
  std::optional<std::string> model;
};

void to_json(nlohmann::json &j, const ResponseRequest &req);

struct Response {
	nlohmann::json output = nlohmann::json::array();
};

void from_json(const nlohmann::json &j, Response &resp);

} // namespace openai
