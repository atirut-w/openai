#pragma once
#include <stdexcept>

namespace openai {

class Error : public std::runtime_error {
public:
  explicit Error(const std::string &message) : std::runtime_error(message) {}
};

} // namespace openai
