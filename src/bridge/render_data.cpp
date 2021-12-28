#include "render_data.h"

let::bridge::render_data_cache::render_data_cache(opengl::manager *gl_manager) : _gl_manager(gl_manager) {
    _meshing_program = _gl_manager->create_program({
        "shaders/chunk_meshing/shader.comp"
    });
}

std::optional<let::bridge::render_data_cache::data> let::bridge::render_data_cache::cached_chunk(uint64_t key) const noexcept {
    auto it = _chunks.find(key);
    if (it == _chunks.end())
        return std::nullopt;
    else
        return it->second;
}

let::bridge::render_data::render_data(const let::world &world, render_data_cache &cache) {
    ZoneScopedN("render_data::render_data");
    // Worst case, each chunk has block_count * 4 vertices
    _vertices.reserve(world._chunks.size() * 65536 * 4);
    _indices.reserve(world._chunks.size() * 65536 * 6);

    for (const auto &render_chunk : world._chunks)
    {

        break;
    }
}
