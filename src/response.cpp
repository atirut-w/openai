#include <format>
#include <openai/error.hpp>
#include <openai/response.hpp>

namespace openai {

void to_json(nlohmann::json &j, const InputTextContent &content) {
  j["type"] = "input_text";
  j["text"] = content.text;
}

void to_json(nlohmann::json &j, const InputImageContent &content) {
  j["type"] = "input_image";
  if (content.image_url) {
    j["image_url"] = *content.image_url;
  } else {
    j["image_url"] = nullptr;
  }
  if (content.file_id) {
    j["file_id"] = *content.file_id;
  } else {
    j["file_id"] = nullptr;
  }
  j["detail"] = content.detail;
}

void to_json(nlohmann::json &j, const InputFileContent &content) {
  j["type"] = "input_file";
  if (content.file_id) {
    j["file_id"] = *content.file_id;
  }
  if (content.filename) {
    j["filename"] = *content.filename;
  }
  if (content.file_url) {
    j["file_url"] = *content.file_url;
  }
  if (content.file_data) {
    j["file_data"] = *content.file_data;
  }
}

void to_json(nlohmann::json &j, const InputContent &content) {
  std::visit([&j](auto &&arg) { j = arg; }, content);
}

void to_json(nlohmann::json &j, const EasyInputMessage &message) {
  j["type"] = "message";
  j["role"] = message.role;
  
  // Assistant messages in input need output content types (output_text, refusal)
  if (message.role == "assistant") {
    if (std::holds_alternative<std::string>(message.content)) {
      nlohmann::json part;
      part["type"] = "output_text";
      part["text"] = std::get<std::string>(message.content);
      part["annotations"] = nlohmann::json::array();
      j["content"] = nlohmann::json::array({part});
    } else {
      const auto &list = std::get<InputMessageContentList>(message.content);
      nlohmann::json arr = nlohmann::json::array();
      for (const auto &c : list) {
        std::visit([&arr](auto &&arg) {
          using T = std::decay_t<decltype(arg)>;
          if constexpr (std::is_same_v<T, InputTextContent>) {
            nlohmann::json part;
            part["type"] = "output_text";
            part["text"] = arg.text;
            part["annotations"] = nlohmann::json::array();
            arr.push_back(part);
          } else {
            // Non-text content in assistant messages - serialize as-is
            nlohmann::json part = arg;
            arr.push_back(part);
          }
        }, c);
      }
      j["content"] = arr;
    }
  } else {
    // User, system, developer messages use input content types
    std::visit([&j](auto &&arg) { j["content"] = arg; }, message.content);
  }
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
  // type field is constant, no need to deserialize
  citation.file_id = j["file_id"].get<std::string>();
  citation.index = j["index"].get<int>();
  citation.filename = j["filename"].get<std::string>();
}

void from_json(const nlohmann::json &j, UrlCitation &citation) {
  // type field is constant, no need to deserialize
  citation.url = j["url"].get<std::string>();
  citation.start_index = j["start_index"].get<int>();
  citation.end_index = j["end_index"].get<int>();
  citation.title = j["title"].get<std::string>();
}

void from_json(const nlohmann::json &j, ContainerFileCitation &citation) {
  // type field is constant, no need to deserialize
  citation.container_id = j["container_id"].get<std::string>();
  citation.file_id = j["file_id"].get<std::string>();
  citation.start_index = j["start_index"].get<int>();
  citation.end_index = j["end_index"].get<int>();
  citation.filename = j["filename"].get<std::string>();
}

void from_json(const nlohmann::json &j, FilePath &file_path) {
  // type field is constant, no need to deserialize
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
  // type field is constant, no need to deserialize
  content.text = j["text"].get<std::string>();
  if (j.contains("annotations") && !j["annotations"].is_null()) {
    content.annotations = j["annotations"].get<std::vector<Annotation>>();
  }
}

void from_json(const nlohmann::json &j, RefusalContent &content) {
  // type field is constant, no need to deserialize
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
  // type and role fields have constant default values, no need to deserialize
  if (j.contains("role") && !j["role"].is_null()) {
    message.role = j["role"].get<std::string>();
  }
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
