#pragma once
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace openai {

struct InputTextContent {
	std::string text;
};

struct InputImageContent {
	std::string image_url;
	std::string file_id;
	std::string detail;
};

struct InputFileContent {
	std::string file_id;
	std::string detail;
};

using InputContent = std::variant<InputTextContent, InputImageContent, InputFileContent>;

using InputMessageContentList = std::vector<InputContent>;

struct EasyInputMessage {
	std::string role;
	std::variant<std::string, InputMessageContentList> content;
};

using InputItem = std::variant<EasyInputMessage>;

using InputParam = std::variant<std::string, std::vector<InputItem>>;

struct CreateResponse {
  InputParam input;
  std::optional<std::string> model;
};

void to_json(nlohmann::json &j, const CreateResponse &req);

struct FileCitation {
	std::string file_id;
	int index;
	std::string filename;
};

struct UrlCitation {
	std::string url;
	int start_index;
	int end_index;
	std::string title;
};

struct ContainerFileCitation {
	std::string container_id;
	std::string file_id;
	int start_index;
	int end_index;
	std::string filename;
};

struct FilePath {
	std::string file_id;
	int index;
};

using Annotation =
    std::variant<FileCitation, UrlCitation, ContainerFileCitation, FilePath>;

struct OutputTextContent {
  std::string text;
  std::vector<Annotation> annotations;
};

struct RefusalContent {
	std::string refusal;
};

using OutputMessageContent = std::variant<OutputTextContent, RefusalContent>;

struct OutputMessage {
  std::string id;
  std::vector<OutputMessageContent> content;
  std::string status;
};

using OutputItem = std::variant<OutputMessage>;

struct Response {
  std::vector<OutputItem> output;
};

void from_json(const nlohmann::json &j, Response &resp);

} // namespace openai
