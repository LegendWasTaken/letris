#ifndef LETRIS_RENDER_DATA_H
#define LETRIS_RENDER_DATA_H

#include <span>
#include <bitset>
#include <random>
#include <unordered_map>

#include <world/world.h>
#include <world/chunk/chunk.h>

#include <renderer/renderer.h>

#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

namespace let::bridge {

    class render_data;

    class render_data_cache {
    public:
        explicit render_data_cache(opengl::manager *gl_manager);

        [[nodiscard]]
        std::optional<uint32_t> allocate(uint64_t key);

    private:
        friend render_data;

        std::uint32_t _meshing_program;

        GLuint _faces;
        GLuint _indirects;
        GLuint _positions;
        GLuint _indices;


        std::unordered_map<uint64_t, uint32_t> _chunks;
        std::unordered_set<uint32_t> _allocated;

        let::opengl::manager *_gl_manager;
    };

    class render_data {
    public:
        explicit render_data(let::world &world, render_data_cache &cache);

        struct chunk_data
        {
            std::optional<uint64_t> chunk_count;
            std::optional<GLuint> positions;
            std::optional<GLuint> faces;
            std::optional<GLuint> indirects;
            std::optional<GLuint> indices;
        };
        [[nodiscard]] chunk_data data();

    private:
        struct to_render {
            let::chunk *chunk;
            uint32_t index;
        };

        void _mesh_chunks(std::span<to_render> chunks, render_data_cache &cache, GLuint faces, GLuint indirects);

        std::optional<uint64_t> _chunk_count;
        std::optional<GLuint> _positions;
        std::optional<GLuint> _faces;
        std::optional<GLuint> _indirects;
        std::optional<GLuint> _indices;
    };
}

#endif //LETRIS_RENDER_DATA_H