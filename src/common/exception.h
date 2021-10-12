#ifndef LETRIS_EXCEPTION_H
#define LETRIS_EXCEPTION_H

#include <string>
#include <exception>
#include <algorithm>
#include <array>
#include <spdlog/fmt/fmt.h>

#define LET_EXCEPTION(type, what, ...) throw let::exception(type, fmt::format(what, ##__VA_ARGS__), __FILE__, __LINE__)

namespace let {
    class exception : std::exception {
    private:
        std::array<std::string, 7> _source_type_lut = {
            "network",
            "render",
            "window",
            "ui",
            "physics",
            "nbt",
            "opengl",
        };
    public:
        // Hey Caio, if you add one here. Remember to update the array above (maybe it's moved), and the test for it
        // thanks, Caio from 4:50am
        enum class source_type {
            network,
            render,
            window,
            ui,
            physics,
            nbt,
            opengl,
        };

        struct source {
            std::string type;
            std::string file;
            std::uint64_t line;
        };

        exception() = delete;

        exception(source_type type, std::string what, std::string file, std::uint64_t line);

        [[nodiscard]] source where() const noexcept;

        [[nodiscard]] char const *what() const override;

    private:
        source _source;
        std::string _what;
    };
}

#endif //LETRIS_EXCEPTION_H
