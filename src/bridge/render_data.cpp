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
        if (const auto &it = cache._chunks.find(key); it != cache._chunks.end()) {
            _chunks[key] = it->second;
        } else {
            cache._gl_manager->bind(cache._meshing_program);

            auto data = render_data_cache::data();
            data.pos.x = render_chunk.second.x;
            data.pos.y = render_chunk.second.z;

            for (int i = 0; i < 16; i++)
            {
                if (render_chunk.second[i] == nullptr)
                    continue;

                auto vao = GLuint();
                auto vbo = GLuint();
                auto ebo = GLuint();
                auto blocks = GLuint();
                auto indirect = GLuint();

                glGenVertexArrays(1, &vao);
                glGenBuffers(1, &vbo);
                glGenBuffers(1, &ebo);
                glGenBuffers(1, &blocks);
                glGenBuffers(1, &indirect);

                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);

                const auto visible_faces = render_chunk.second[i]->visible_faces();

                glBufferData(GL_ARRAY_BUFFER, visible_faces * 4 * 12 * sizeof(float), nullptr, GL_STREAM_COPY);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), nullptr);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (GLvoid *) (4 * sizeof(float)));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (GLvoid *) (8 * sizeof(float)));
                glBindVertexArray(0);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, visible_faces * 6 * sizeof(uint32_t), nullptr, GL_STREAM_COPY);

                struct {
                    uint32_t count = 0;
                    uint32_t instanceCount = 1;
                    uint32_t firstIndex = 0;
                    int32_t baseVertex = 0;
                    uint32_t reservedMustBeZero = 0;
                    uint32_t vertex_count = 0;
                } indirect_base;

                glBindBuffer(GL_SHADER_STORAGE_BUFFER, indirect);
                glBufferData(GL_SHADER_STORAGE_BUFFER, 6 * sizeof(uint32_t), &indirect_base, GL_STREAM_COPY);

                glBindBuffer(GL_SHADER_STORAGE_BUFFER, blocks);
                glBufferData(GL_SHADER_STORAGE_BUFFER, 16 * 16 * 16 * sizeof(uint32_t), render_chunk.second[i]->blocks.data(), GL_STREAM_COPY);

                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo);
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ebo);
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, indirect);
                glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, blocks);

                glDispatchCompute(4, 4, 4);
                glMemoryBarrier(GL_ALL_BARRIER_BITS);

                data.vao[i] = vao;
                data.vbo[i] = vbo;
                data.ebo[i] = ebo;
                data.indirect[i] = indirect;
            }

            cache._chunks[key] = data;

            _chunks[key] = data;
        }
    }
}

let::bridge::render_data::chunk_data let::bridge::render_data::data() {
    auto chunks = chunk_data();
    chunks.positions.reserve(_chunks.size());
    chunks.vertices.reserve(_chunks.size());
    chunks.indices.reserve(_chunks.size());
    chunks.indirects.reserve(_chunks.size());

    for (auto it : _chunks) {
        const auto data = it.second;
        chunks.positions.emplace_back(data.pos.x, data.pos.y);
        chunks.vertices.push_back(data.vao);
        chunks.indices.push_back(data.ebo);
        chunks.indirects.push_back(data.indirect);
    }

    return chunks;
}
