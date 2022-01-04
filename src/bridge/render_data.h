#ifndef LETRIS_RENDER_DATA_H
#define LETRIS_RENDER_DATA_H

#include <span>
#include <bitset>
#include <random>
#include <unordered_map>

#include <world/world.h>
#include <world/chunk/chunk.h>

#include <renderer/renderer.h>

#include <common/task_executor.h>

#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

namespace let::bridge {

    class render_data;

    class render_data_cache {
    public:
        struct data
        {
            glm::ivec2 pos;
            std::optional<GLuint> faces;
            std::optional<GLuint> indirect;
        };

        explicit render_data_cache(opengl::manager *gl_manager);

        [[nodiscard]]
        std::optional<data> cached_chunk(uint64_t key) const noexcept;

    private:
        friend render_data;

        std::uint32_t _meshing_program;

        std::unordered_map<uint64_t, data> _chunks;

        let::opengl::manager *_gl_manager;
    };

    class render_data {
    public:
        explicit render_data(const let::world &world, render_data_cache &cache);

        struct chunk_data
        {
            std::vector<glm::ivec2> positions;
            std::vector<std::optional<GLuint>> faces;
            std::vector<std::optional<GLuint>> indirects;
        };
        [[nodiscard]] chunk_data data();

    private:
        void _mesh_chunk(const let::chunk &chunk, render_data_cache::data &data, render_data_cache &cache);

        std::unordered_map<uint64_t, render_data_cache::data> _chunks;

    };
}

#endif //LETRIS_RENDER_DATA_H