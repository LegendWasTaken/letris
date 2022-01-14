#include "render_data.h"

let::bridge::render_data_cache::render_data_cache(opengl::manager *gl_manager) : _gl_manager(gl_manager) {
    _meshing_program = _gl_manager->create_program({
                                                           "shaders/chunk_meshing/shader.comp"
                                                   });

    // worst case is 32,768 blocks per chunk
    // 196,608 faces per chunk
    // 86,507,520 faces in total
    // 330MB for the entire buffer

    glGenBuffers(1, &_faces);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _faces);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 196'608 * 1000 * sizeof(uint32_t), nullptr, GL_STREAM_COPY);

    struct indirect {
        uint32_t vertex_count = 0;
        uint32_t instance_count = 1;
        uint32_t first_index = 0;
        uint32_t base_index = 0;
    };
    auto indirects = std::vector<indirect>(1000);
    glGenBuffers(1, &_indirects);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _indirects);
    glBufferData(GL_SHADER_STORAGE_BUFFER, indirects.size() * sizeof(indirect), indirects.data(), GL_STREAM_COPY);

    glGenBuffers(1, &_positions);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _positions);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 1000 * sizeof(glm::ivec4), nullptr, GL_STREAM_COPY);

    glGenBuffers(1, &_indices);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _indices);
    glBufferData(GL_SHADER_STORAGE_BUFFER, 1000 * sizeof(uint32_t), nullptr, GL_STREAM_COPY);
}

std::optional<uint32_t> let::bridge::render_data_cache::allocate(uint64_t key) {
    auto it = _chunks.find(key);

    if (_chunks.size() == 1000 || it != _chunks.end())
    {
//        spdlog::debug("already allocated {}", key);
        return std::nullopt;
    }

    for (auto i = 0; i < 1000; i++) {
        const auto allocation = i * 196608;
        if (_allocated.find(allocation) == _allocated.end())
        {
            _allocated.insert(allocation);
            _chunks.insert({key, allocation});
            return allocation;
        }
    }

    return std::nullopt;
}

let::bridge::render_data::render_data(let::world &world, render_data_cache &cache) {
    ZoneScopedN("render_data::render_data");
    cache._gl_manager->bind(cache._meshing_program);
    auto to_update = std::vector<to_render>();

    for (auto &render_chunk : world._chunks) {
        const auto key = chunk::key(render_chunk.second);

        const auto &it = cache._chunks.find(key);
        const auto found = it != cache._chunks.end();
        const auto rerender = render_chunk.second.rerender_any();

        if (!found || rerender)
        {
            auto allocation = uint32_t();
            if (!found)
            {
                const auto attempted_allocation =
                        cache.allocate(key);
                if (!attempted_allocation.has_value())
                    continue;
                allocation = attempted_allocation.value();
            }
            else
                allocation = it->second;

            to_update.push_back(to_render {
                    .chunk = &render_chunk.second,
                    .index = allocation
            });
            render_chunk.second.rerendered();
        }
    }

    if (!to_update.empty())
    {
        spdlog::debug("meshing {} chunks", to_update.size());
        _mesh_chunks(to_update, cache, cache._faces, cache._indirects);
    }

    auto positions = std::vector<glm::ivec4>(1000);
    auto indices = std::vector<uint32_t>(1000);
    for (auto &it : cache._chunks)
    {
        auto pos = chunk::decompose_key(it.first);
        positions[it.second / (196608)] = glm::ivec4(pos, 0, 0);
//        indices[it.second / (196608)] = it.second;
    }

    if (!cache._chunks.empty())
    {
        _faces = cache._faces;
        _indirects = cache._indirects;
        _positions = cache._positions;
        _indices = cache._indices;
        _chunk_count = positions.size();

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, cache._positions);
        glBufferData(GL_SHADER_STORAGE_BUFFER, positions.size() * sizeof(glm::ivec4), positions.data(), GL_STREAM_COPY);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, cache._indices);
        glBufferData(GL_SHADER_STORAGE_BUFFER, indices.size() * sizeof(uint32_t), positions.data(), GL_STREAM_COPY);
    }
}

let::bridge::render_data::chunk_data let::bridge::render_data::data() {
    auto chunks = chunk_data();
    chunks.chunk_count = _chunk_count;
    chunks.positions = _positions;
    chunks.faces = _faces;
    chunks.indirects = _indirects;
    chunks.indices = _indices;

    return chunks;
}

void let::bridge::render_data::_mesh_chunks(std::span<to_render> chunks, render_data_cache &cache, GLuint faces, GLuint indirects) {
    ZoneScopedN("mesh chunk");

    auto block_data = std::vector<uint32_t>(chunks.size() * 16 * 256 * 16);
    auto indices = std::vector<uint32_t>(chunks.size());

    auto c = std::vector<to_render>(chunks.begin(), chunks.end());

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, indirects);
    for (auto i = 0; i < chunks.size(); i++) {
        auto &chunk = *chunks[i].chunk;
        indices[i] = chunks[i].index;

        for (auto j = 0; j < 16; j++) {
            if (chunk[j] != nullptr) {
                const auto base_chunk_index = i * 16 * 256 * 16;
                const auto index = base_chunk_index + j * 16 * 16 * 16;
                std::memcpy(block_data.data() + index, chunk[j]->blocks.data(), 16 * 16 * 16 * sizeof(uint32_t));
            }
        }

        struct {
            uint32_t vertex_count = 0;
            uint32_t instance_count = 1;
            uint32_t first_index = 0;
            uint32_t base_index = 0;
        } indirect;
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, (indices[i] / 196608) * sizeof(uint32_t) * 4, sizeof(uint32_t) * 4, &indirect);
    }

    auto ogl_blocks = GLuint();
    auto ogl_indices = GLuint();
    glGenBuffers(1, &ogl_blocks);
    glGenBuffers(1, &ogl_indices);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ogl_blocks);
    glBufferData(GL_SHADER_STORAGE_BUFFER, block_data.size() * sizeof(uint32_t), block_data.data(), GL_STREAM_COPY);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ogl_indices);
    glBufferData(GL_SHADER_STORAGE_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STREAM_COPY);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, faces);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, indirects);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ogl_blocks);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ogl_indices);

    TracyGpuZone("mesh::compute");
    glDispatchCompute(chunks.size(), 32, 2);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glDeleteBuffers(1, &ogl_blocks);
    glDeleteBuffers(1, &ogl_indices);
}
