#pragma once
#include <curl/curl.h>

namespace openai {

class OpenAI {
  CURL *curl;

public:
  OpenAI();
  ~OpenAI();

  // TODO: Implement copy and move
  OpenAI(const OpenAI &other) = delete;
  OpenAI &operator=(const OpenAI &other) = delete;
  OpenAI(OpenAI &&other) = delete;
  OpenAI &operator=(OpenAI &&other) = delete;
};

} // namespace openai
