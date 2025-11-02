#pragma once
#include <curl/curl.h>
#include <optional>
#include <string>

namespace openai {

class OpenAI {
  CURL *curl = nullptr;
  curl_slist *headers = nullptr;

public:
  OpenAI(std::optional<std::string> api_key = std::nullopt);
  ~OpenAI();

  // TODO: Implement copy and move
  OpenAI(const OpenAI &other) = delete;
  OpenAI &operator=(const OpenAI &other) = delete;
  OpenAI(OpenAI &&other) = delete;
  OpenAI &operator=(OpenAI &&other) = delete;
};

} // namespace openai
