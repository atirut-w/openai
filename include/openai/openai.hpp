#pragma once
#include <curl/curl.h>
#include <openai/response.hpp>
#include <optional>
#include <string>

namespace openai {

class OpenAI {
  CURL *curl = nullptr;
  curl_slist *headers = nullptr;

  std::string http_get(const std::string &url);
  std::string http_post(const std::string &url, const std::string &data);

public:
  OpenAI(std::optional<std::string> api_key = std::nullopt);
  ~OpenAI();

  // TODO: Implement copy and move
  OpenAI(const OpenAI &other) = delete;
  OpenAI &operator=(const OpenAI &other) = delete;
  OpenAI(OpenAI &&other) = delete;
  OpenAI &operator=(OpenAI &&other) = delete;

  Response create_response(const ResponseRequest &req);
};

} // namespace openai
