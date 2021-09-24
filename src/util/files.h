#ifndef LETRIS_FILES_H
#define LETRIS_FILES_H

#include <fstream>
#include <sstream>
#include <filesystem>

namespace let {
    [[nodiscard]] inline std::string read_file(const std::filesystem::path &path) {
        auto file_buffer = std::ifstream(path);
        auto string_buffer = std::stringstream();
        return string_buffer << file_buffer.rdbuf(), string_buffer.str();
    }
}

#endif //LETRIS_FILES_H
