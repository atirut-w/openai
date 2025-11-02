#include <openai/openai.hpp>
#include <sstream>

static size_t write_callback(char *ptr, size_t size, size_t nmemb,
                             std::stringstream *userdata) {
  userdata->write(ptr, size * nmemb);
  return size * nmemb;
}

namespace openai {

std::string OpenAI::http_get(const std::string &url) {
  std::stringstream response_stream;

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_stream);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    throw std::runtime_error("CURL GET request failed: " +
                             std::string(curl_easy_strerror(res)));
  }

  return response_stream.str();
}

std::string OpenAI::http_post(const std::string &url, const std::string &data) {
  std::stringstream response_stream;

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1L);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_stream);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    throw std::runtime_error("CURL POST request failed: " +
                             std::string(curl_easy_strerror(res)));
  }

  return response_stream.str();
}

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
    std::string auth_header =
        "Authorization: Bearer " + std::string(env_api_key);
    headers = curl_slist_append(headers, auth_header.c_str());
  }

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
}

OpenAI::~OpenAI() {
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
}

Response OpenAI::create_response(const ResponseRequest &req) {
  return nlohmann::json::parse(http_post("https://api.openai.com/v1/responses",
                                         nlohmann::json(req).dump()))
      .get<Response>();
}

} // namespace openai
