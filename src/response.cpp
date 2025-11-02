#include <format>
#include <openai/error.hpp>
#include <openai/response.hpp>

namespace openai {

void to_json(nlohmann::json &j, const ResponseRequest &req) {
  if (req.input) {
    j["input"] = *req.input;
  }
  if (req.model) {
    j["model"] = *req.model;
  }
}

void from_json(const nlohmann::json &j, Response &resp) {
  if (j.contains("error") && !j["error"].is_null()) {
    throw Error(std::format("{}: {}", j["error"]["code"].get<std::string>(),
                            j["error"]["message"].get<std::string>()));
  }
}

} // namespace openai
