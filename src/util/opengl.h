#ifndef LETRIS_OPENGL_H
#define LETRIS_OPENGL_H

#include <fstream>
#include <sstream>
#include <cstdint>
#include <exception>
#include <array>

#include <glad/glad.h>

namespace let::opengl {
    namespace detail {
        [[nodiscard]] inline std::uint32_t shader_type_from_path(std::string_view path) {
            if (path.ends_with(".comp"))
                return GL_COMPUTE_SHADER;
            else if (path.ends_with(".frag"))
                return GL_FRAGMENT_SHADER;
            else if (path.ends_with(".vert"))
                return GL_VERTEX_SHADER;
            throw std::exception("Couldn't figure out shader path");
        }
    }

    class shader_compile_exception : public std::exception {
        std::string _path;
        std::string _error;
    public:
        shader_compile_exception(std::string_view path, const char *error) :
            _path(path), _error(error)
        {
        }

        [[nodiscard]] const std::string& path() const noexcept {
            return _path;
        }

        [[nodiscard]] const std::string& error() const noexcept {
            return _error;
        }
    };
    [[nodiscard]] inline std::uint32_t create_shader(std::string_view path) {
        ZoneScopedN("opengl::create_shader");

        auto shader_stream = std::ifstream(std::string(path));
        auto shader_string_stream = std::stringstream();
        shader_string_stream << shader_stream.rdbuf();

        const auto shader_contents = shader_string_stream.str();
        const auto shader_c_str = shader_contents.c_str();

        const auto handle = glCreateShader(detail::shader_type_from_path(path));
        glShaderSource(handle, 1, &shader_c_str, nullptr);
        glCompileShader(handle);

        auto success = std::int32_t(0);
        auto log = std::array<char, 513>();
        glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(handle, 512, nullptr, log.data());
            throw shader_compile_exception(path, log.data());
        }

        return handle;
    }


    class program_link_exception : public std::exception {
        std::uint32_t _pid;
        std::string _error;
    public:
        program_link_exception(std::uint32_t pid, const char *error) :
        _pid(pid), _error(error)
        {
        }

        [[nodiscard]] const std::uint32_t & pid() const noexcept {
            return _pid;
        }

        [[nodiscard]] const std::string& error() const noexcept {
            return _error;
        }
    };
    template <typename ...Shaders>
    [[nodiscard]] inline GLuint create_program(Shaders&&... shaders)
    {
        ZoneScopedN("opengl::create_program");
        const auto handle = glCreateProgram();

        for (const auto shader : {shaders...})
            glAttachShader(handle, shader);

        glLinkProgram(handle);

        auto success = int(0);
        auto log = std::array<char, 513>();
        glGetProgramiv(handle, GL_LINK_STATUS, &success);

        if (!success)
        {
            glGetProgramInfoLog(handle, 512, nullptr, log.data());
            throw program_link_exception(handle, log.data());
        }

        return handle;
    }
}

#endif //LETRIS_OPENGL_H
