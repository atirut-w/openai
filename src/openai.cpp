#include <openai/openai.hpp>
#include <sstream>

static size_t write_callback(char *ptr, size_t size, size_t nmemb,
                             std::stringstream *userdata) {
  userdata->write(ptr, size * nmemb);
  return size * nmemb;
}

namespace openai {

OpenAI::OpenAI(std::optional<std::string> api_key) {
  curl = curl_easy_init();

  headers = curl_slist_append(headers, "Content-Type: application/json");
  if (api_key) {
    std::string auth_header = "Authorization: Bearer " + *api_key;
    headers = curl_slist_append(headers, auth_header.c_str());
  } else {
    const char *env_api_key = std::getenv("OPENAI_API_KEY");
    if (!env_api_key) {
      throw std::runtime_error(
          "API key not provided and OPENAI_API_KEY not set in environment");
    }
    std::string auth_header = "Authorization: Bearer " + std::string(env_api_key);
    headers = curl_slist_append(headers, auth_header.c_str());
  }

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
}

OpenAI::~OpenAI() {
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
}

} // namespace openai
