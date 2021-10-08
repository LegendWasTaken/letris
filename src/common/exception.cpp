#include "exception.h"

let::exception::exception(let::exception::source_type type, std::string what, std::string file,
                          std::uint64_t line) {

    // We create a copy, because file is the full path
    // so we do some string parsing to get the file
    auto file_stem = std::string();
    file_stem.reserve(file.size());
    for (size_t i = file.size() - 1; i < file.size(); i--) {
        if (file[i] == '/' || file[i] == '\\')
            break;
        else
            file_stem.push_back(file[i]);
    }

    std::reverse(file_stem.begin(), file_stem.end());

    _source = {
            .type = _source_type_lut[static_cast<int>(type)],
            .file = std::move(file_stem),
            .line = line
    };
    _what = std::move(what);
}

let::exception::source let::exception::where() const noexcept {
    return _source;
}

char const *let::exception::what() const {
    return _what.data();
}
