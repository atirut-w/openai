#include <format>
#include <openai/error.hpp>
#include <openai/response.hpp>

namespace openai {

void to_json(nlohmann::json &j, const InputTextContent &content) {
  j["text"] = content.text;
}

void to_json(nlohmann::json &j, const InputImageContent &content) {
  j["image_url"] = content.image_url;
  j["file_id"] = content.file_id;
  j["detail"] = content.detail;
}

void to_json(nlohmann::json &j, const InputFileContent &content) {
  j["file_id"] = content.file_id;
  j["detail"] = content.detail;
}

void to_json(nlohmann::json &j, const InputContent &content) {
  std::visit([&j](auto &&arg) { j = arg; }, content);
}

void to_json(nlohmann::json &j, const EasyInputMessage &message) {
  j["role"] = message.role;
  std::visit([&j](auto &&arg) { j["content"] = arg; }, message.content);
}

void to_json(nlohmann::json &j, const InputItem &item) {
  std::visit([&j](auto &&arg) { j = arg; }, item);
}

void to_json(nlohmann::json &j, const InputParam &param) {
  std::visit([&j](auto &&arg) { j = arg; }, param);
}

void to_json(nlohmann::json &j, const CreateResponse &req) {
  j["input"] = req.input;
  if (req.model) {
    j["model"] = *req.model;
  }
}

void from_json(const nlohmann::json &j, FileCitation &citation) {
  citation.file_id = j["file_id"].get<std::string>();
  citation.index = j["index"].get<int>();
  citation.filename = j["filename"].get<std::string>();
}

void from_json(const nlohmann::json &j, UrlCitation &citation) {
  citation.url = j["url"].get<std::string>();
  citation.start_index = j["start_index"].get<int>();
  citation.end_index = j["end_index"].get<int>();
  citation.title = j["title"].get<std::string>();
}

void from_json(const nlohmann::json &j, ContainerFileCitation &citation) {
  citation.container_id = j["container_id"].get<std::string>();
  citation.file_id = j["file_id"].get<std::string>();
  citation.start_index = j["start_index"].get<int>();
  citation.end_index = j["end_index"].get<int>();
  citation.filename = j["filename"].get<std::string>();
}

void from_json(const nlohmann::json &j, FilePath &file_path) {
  file_path.file_id = j["file_id"].get<std::string>();
  file_path.index = j["index"].get<int>();
}

void from_json(const nlohmann::json &j, Annotation &annotation) {
  std::string type = j["type"].get<std::string>();
  if (type == "file_citation") {
    annotation = j.get<FileCitation>();
  } else if (type == "url_citation") {
    annotation = j.get<UrlCitation>();
  } else if (type == "container_file_citation") {
    annotation = j.get<ContainerFileCitation>();
  } else if (type == "file_path") {
    annotation = j.get<FilePath>();
  } else {
    throw Error(std::format("Unknown Annotation type: {}", type));
  }
}

void from_json(const nlohmann::json &j, OutputTextContent &content) {
  content.text = j["text"].get<std::string>();
  if (j.contains("annotations")) {
    content.annotations = j["annotations"].get<std::vector<Annotation>>();
  }
}

void from_json(const nlohmann::json &j, RefusalContent &content) {
  content.refusal = j["refusal"].get<std::string>();
}

void from_json(const nlohmann::json &j, OutputMessageContent &content) {
  std::string type = j["type"].get<std::string>();
  if (type == "output_text") {
    content = j.get<OutputTextContent>();
  } else if (type == "refusal") {
    content = j.get<RefusalContent>();
  } else {
    throw Error(std::format("Unknown OutputMessageContent type: {}", type));
  }
}

void from_json(const nlohmann::json &j, OutputMessage &message) {
  message.id = j["id"].get<std::string>();
  message.status = j["status"].get<std::string>();
  message.content = j["content"].get<std::vector<OutputMessageContent>>();
}

void from_json(const nlohmann::json &j, OutputItem &item) {
  std::string type = j["type"].get<std::string>();
  if (type == "message") {
    item = j.get<OutputMessage>();
  } else {
    throw Error(std::format("Unknown OutputItem type: {}", type));
  }
}

void from_json(const nlohmann::json &j, Response &resp) {
  if (j.contains("error") && !j["error"].is_null()) {
    throw Error(std::format("{}: {}", j["error"]["code"].get<std::string>(),
                            j["error"]["message"].get<std::string>()));
  }
  resp.output = j["output"].get<std::vector<OutputItem>>();
}

} // namespace openai
