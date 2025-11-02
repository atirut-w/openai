#pragma once
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace openai {

struct ResponseRequest {
  std::optional<std::variant<std::string, std::vector<nlohmann::json>>> input;
  std::optional<std::string> model;
};

void to_json(nlohmann::json &j, const ResponseRequest &req);

struct Response {
  std::vector<nlohmann::json> output;
};

void from_json(const nlohmann::json &j, Response &resp);

} // namespace openai
