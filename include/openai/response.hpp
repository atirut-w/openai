#pragma once
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace openai {

struct ResponseRequest {
	std::optional<std::string> input;
	std::optional<std::string> model;
};

void to_json(nlohmann::json &j, const ResponseRequest &req);

struct Response {};

void from_json(const nlohmann::json &j, Response &resp);

} // namespace openai
