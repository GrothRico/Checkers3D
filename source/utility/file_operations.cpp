#include "file_operations.hpp"
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

std::optional<std::string>
util::read_file_as_string(const std::string &filepath) {
  std::ifstream file_stream{};
  file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  std::stringstream stream{};
  try {
    file_stream.open(filepath);
    stream << file_stream.rdbuf();
    file_stream.close();
  } catch (std::ifstream::failure failure) {
    return {};
  }
  return stream.str();
}
