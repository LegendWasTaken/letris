#include "render_data.h"

let::bridge::render_data_cache::render_data_cache(opengl::manager *gl_manager) : _gl_manager(gl_manager) {
    _meshing_program = _gl_manager->create_program({
                                                           "shaders/chunk_meshing/shader.comp"
                                                   });
}

std::optional<let::bridge::render_data_cache::data>
let::bridge::render_data_cache::cached_chunk(uint64_t key) const noexcept {
    auto it = _chunks.find(key);
    if (it == _chunks.end())
        return std::nullopt;
    else
        return it->second;
}

let::bridge::render_data::render_data(const let::world &world, render_data_cache &cache) {
    ZoneScopedN("render_data::render_data");

    for (const auto &render_chunk : world._chunks) {
        const auto key = chunk::key(render_chunk.second);
        const auto rerender = render_chunk.second.rerender_any();

        const auto &it = cache._chunks.find(key);
        const auto found = it != cache._chunks.end();

        if (found && !rerender) {
            _chunks[key] = it->second;
        } else if (found && rerender) {
            auto data = _chunks[key];
            for (auto i = 0; i < 16; i++)
            {
                if (!data.indirect[i].has_value())
                    continue;

                glDeleteBuffers(1, &data.faces[i].value());
                glDeleteBuffers(1, &data.indirect[i].value());
            }
            _mesh_chunk(render_chunk.second, data, cache);
            render_chunk.second.rerendered();

            // Todo: replace this with an iterator / reference so 2 lookups instead of 3
            cache._chunks[key] = data;
            _chunks[key] = data;
        } else {
            auto data = render_data_cache::data();

            _mesh_chunk(render_chunk.second, data, cache);

            cache._chunks[key] = data;

            _chunks[key] = data;
        }
    }
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

let::bridge::render_data::chunk_data let::bridge::render_data::data() {
    auto chunks = chunk_data();
    chunks.positions.reserve(_chunks.size());
    chunks.faces.reserve(_chunks.size());
    chunks.indirects.reserve(_chunks.size());

    for (auto it : _chunks) {
        const auto data = it.second;
        chunks.positions.emplace_back(data.pos.x, data.pos.y);
        chunks.faces.push_back(data.faces);
        chunks.indirects.push_back(data.indirect);
    }

    return chunks;
}

void let::bridge::render_data::_mesh_chunk(const let::chunk &chunk, render_data_cache::data &data, render_data_cache &cache) {
    ZoneScopedN("mesh chunk");
    cache._gl_manager->bind(cache._meshing_program);

    for (int i = 0; i < 16; i++)
    {
        if (chunk[i] == nullptr)
            continue;

        const auto visible_faces = chunk[i]->visible_faces();

        if (visible_faces == 0)
            continue;

        auto faces = GLuint();
        auto blocks = GLuint();
        auto indirect = GLuint();

        glGenBuffers(1, &faces);
        glGenBuffers(1, &blocks);
        glGenBuffers(1, &indirect);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, faces);
        glBufferData(GL_SHADER_STORAGE_BUFFER, visible_faces * sizeof(int32_t) * 4, nullptr, GL_STREAM_COPY);

        struct {
            uint32_t count = 0;
            uint32_t instanceCount = 1;
            uint32_t firstIndex = 0;
            int32_t baseVertex = 0;
            uint32_t reservedMustBeZero = 0;
        } indirect_base;

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, indirect);
        glBufferData(GL_SHADER_STORAGE_BUFFER, 5 * sizeof(uint32_t), &indirect_base, GL_STREAM_COPY);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, blocks);
        glBufferData(GL_SHADER_STORAGE_BUFFER, 16 * 16 * 16 * sizeof(uint32_t), chunk[i]->blocks.data(), GL_STREAM_COPY);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, faces);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, indirect);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, blocks);

        TracyGpuZone("mesh::compute");
        glDispatchCompute(4, 4, 4);

        data.faces[i] = faces;
        data.indirect[i] = indirect;
    }
    data.pos.x = chunk.x;
    data.pos.y = chunk.z;
}
