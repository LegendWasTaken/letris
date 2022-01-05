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

    auto faces = std::vector<std::pair<glm::ivec2, std::vector<glm::ivec4>>>();

    auto data = std::vector<render_data_cache::data>();

    for (const auto &render_chunk : world._chunks) {
        const auto key = chunk::key(render_chunk.second);
        const auto rerender = render_chunk.second.rerender_any();

        const auto &it = cache._chunks.find(key);
        const auto found = it != cache._chunks.end();

        if (found && !rerender)
            data.push_back(it->second);
        else
        {
            auto cached = std::optional<render_data_cache::data>();

            if (cached.has_value() && cached->indirect.has_value()) {
                glDeleteBuffers(1, &cached->faces.value());
                glDeleteBuffers(1, &cached->indirect.value());
            }

            auto chunk_faces = _mesh_chunk(render_chunk.second);
            render_chunk.second.rerendered();

            faces.emplace_back(glm::ivec2(render_chunk.second.x, render_chunk.second.z),
                               std::move(chunk_faces));
        }
    }

    for (const auto &[position, chunk_faces] : faces)
    {
        auto ogl_faces = GLuint();
        auto ogl_indirect = GLuint();

        glGenBuffers(1, &ogl_faces);
        glGenBuffers(1, &ogl_indirect);

        struct {
            uint32_t count = 0;
            uint32_t instanceCount = 1;
            uint32_t firstIndex = 0;
            int32_t baseVertex = 0;
            uint32_t reservedMustBeZero = 0;
            uint32_t p0 = 0;
            uint32_t p1 = 0;
            uint32_t p2 = 0;
        } indirect_base;
        indirect_base.count = chunk_faces.size() * 6;

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ogl_faces);
        glBufferData(GL_SHADER_STORAGE_BUFFER,  chunk_faces.size() * sizeof(int32_t) * 4, chunk_faces.data(), GL_STREAM_COPY);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ogl_indirect);
        glBufferData(GL_SHADER_STORAGE_BUFFER, 8 * sizeof(uint32_t), &indirect_base, GL_STREAM_COPY);

        data.push_back(render_data_cache::data {
            .pos = position,
            .faces = ogl_faces,
            .indirect = ogl_indirect
        });
    }

    for (const auto &d : data)
    {
        const auto key = chunk::key(d.pos.x, d.pos.y);
        cache._chunks[key] = d;
        _chunks[key] = d;
    }
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

std::vector<glm::ivec4> let::bridge::render_data::_mesh_chunk(const let::chunk &chunk) {
    ZoneScopedN("mesh chunk");
    auto total_visible = size_t(0);
    auto is_visible = std::bitset<16>();
    for (int i = 0; i < 16; i++)
    {
        const auto faces = chunk[i] != nullptr ? chunk[i]->visible_faces() : 0;
        total_visible += faces;
        is_visible[i] = faces > 0;
    }

    const auto should_render = total_visible > 0;

    auto data = render_data_cache::data();
    data.pos.x = chunk.x;
    data.pos.y = chunk.z;

    if (should_render)
    {
        auto face_buffer = std::vector<glm::ivec4>();
        face_buffer.reserve(total_visible);

        for (auto i = 0; i < 16; i++)
        {
            if (!is_visible[i])
                continue;

            auto section = chunk[i];

            for (auto y = 0; y < 16; y++)
                for (auto z = 0; z < 16; z++)
                    for (auto x = 0; x < 16; x++)
                    {
                        auto b = section->get_block(x, y, z);
                        auto make_face = [](glm::ivec3 bp, uint32_t direction, uint32_t id) {
                            uint32_t meta = (direction << 12) & 0xF000;
                            meta |= (id << 16) & 0xFFFF0000;
                            int32_t meta_as_int;
                            std::memcpy(&meta_as_int, &meta, sizeof(uint32_t));
                            return glm::ivec4(bp, static_cast<int32_t>(meta_as_int));
                        };

                        for (auto j = 0; j < 6; j++)
                            if (b.visible(static_cast<block::face>(j)))
                                face_buffer.push_back(make_face({x, y + i * 16, z}, j, b.id()));
                    }
        }

        return face_buffer;
    }
    return {};
}
