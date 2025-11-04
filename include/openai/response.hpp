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
	std::optional<std::string> image_url;
	std::optional<std::string> file_id;
	std::string detail = "auto"; // low, high, or auto
};

struct InputFileContent {
	std::optional<std::string> file_id;
	std::optional<std::string> filename;
	std::optional<std::string> file_url;
	std::optional<std::string> file_data;
};

using InputContent = std::variant<InputTextContent, InputImageContent, InputFileContent>;

using InputMessageContentList = std::vector<InputContent>;

struct EasyInputMessage {
	std::string role; // user, assistant, system, or developer
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
	std::string role;
  std::vector<OutputMessageContent> content;
  std::string status; // in_progress, completed, or incomplete

	operator EasyInputMessage() const {
		EasyInputMessage msg;
			msg.role = "assistant";
		InputMessageContentList content_list;

		for (const auto &item : content) {
			if (std::holds_alternative<OutputTextContent>(item)) {
				const auto &text_content = std::get<OutputTextContent>(item);
				InputTextContent input_text;
				input_text.text = text_content.text;
				content_list.push_back(input_text);
			} else if (std::holds_alternative<RefusalContent>(item)) {
				const auto &refusal_content = std::get<RefusalContent>(item);
				InputTextContent input_text;
				input_text.text = std::string("Refusal: ") + refusal_content.refusal;
				content_list.push_back(input_text);
			}
		}

		msg.content = content_list;
		return msg;
	}
};

using OutputItem = std::variant<OutputMessage>;

struct Response {
  std::vector<OutputItem> output;
};

void from_json(const nlohmann::json &j, Response &resp);

} // namespace openai
