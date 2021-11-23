#ifndef LETRIS_RENDER_DATA_H
#define LETRIS_RENDER_DATA_H

#include <span>
#include <random>

#include <world/world.h>

#include <tracy/Tracy.hpp>

namespace let::bridge {
    class render_data {
    public:

        struct vertex
        {
            glm::vec3 pos;
            glm::vec3 col;
            glm::vec3 normal;
        };

        explicit render_data(const let::world &world);

        [[nodiscard]] std::span<const vertex> vertices() const noexcept;

        [[nodiscard]] std::span<const std::uint32_t> indices() const noexcept;

    private:

        enum class block_face
        {
            north,
            east,
            south,
            west,
            top,
            bottom,
        };

        void _add_face(glm::ivec2 chunk, glm::ivec3 block_pos, block_face face);

        std::vector<vertex> _vertices;
        std::vector<std::uint32_t> _indices;
    };
}

#endif //LETRIS_RENDER_DATA_H