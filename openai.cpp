#include <openai/openai.hpp>

namespace openai {

OpenAI::OpenAI() {
  curl = curl_easy_init();
}

OpenAI::~OpenAI() {
  curl_easy_cleanup(curl);
}

} // namespace openai
