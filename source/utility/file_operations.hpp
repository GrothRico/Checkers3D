#pragma once

#include <string>
#include <optional>

namespace util {
    std::optional<std::string> read_file_as_string(const std::string& filepath);
}
