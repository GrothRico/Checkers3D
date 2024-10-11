#pragma once

#include <optional>
#include <string>

namespace util {
std::optional<std::string> read_file_as_string(const std::string &filepath);
}
