#ifndef LETRIS_MANAGER_H
#define LETRIS_MANAGER_H

#include <cstdint>
#include <string>
#include <array>
#include <optional>
#include <span>
#include <initializer_list>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <tracy/Tracy.hpp>

#include <util/files.h>

#include <common/exception.h>

namespace let::opengl {
    class manager {
    public:
        manager() = default;

        ~manager();

        [[nodiscard]] std::uint64_t create_program(const std::initializer_list<std::string> &shaders);

        void bind(std::uint64_t program);

        void uniform(const std::string &name, float value);

        void uniform(const std::string &name, glm::vec2 value);

        void uniform(const std::string &name, glm::vec3 value);

        void uniform(const std::string &name, glm::vec4 value);

        void uniform(const std::string &name, int value);

        void uniform(const std::string &name, glm::ivec2 value);

        void uniform(const std::string &name, glm::ivec3 value);

        void uniform(const std::string &name, glm::ivec4 value);

        void uniform(const std::string &name, glm::mat4 value);

    private:
        [[nodiscard]] std::int32_t _uniform_location(const std::string &name);

        [[nodiscard]] static std::uint32_t _create_shader(const std::string &path);

        struct program {
            std::uint32_t handle = ~0;
            std::array<std::optional<std::uint32_t>, 4> shaders;
        };

        [[nodiscard]] static program _create_program(const std::array<std::optional<std::uint32_t>, 4> &shaders);

        std::uint64_t _current_program;

        std::vector<program> _programs;
    };
}

#endif //LETRIS_MANAGER_H
