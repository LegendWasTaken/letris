#include "render_data.h"

let::bridge::render_data::render_data(const let::world &world) {
    ZoneScopedN("render_data::render_data");

    auto e2 = std::mt19937();
    std::uniform_real_distribution<float> dist(0.0, 1.0);
    for (const auto &it : world._chunks) {
        const auto &chunk = it.second;

        for (auto i = 0; i < 16; i++) {
            if (chunk[i] == nullptr)
                continue;

        }

        break;
    }
}

std::span<const let::bridge::render_data::vertex> let::bridge::render_data::vertices() const noexcept {
    return _vertices;
}

std::span<const std::uint32_t> let::bridge::render_data::indices() const noexcept {
    return _indices;
}

void
let::bridge::render_data::_add_face(glm::ivec2 chunk, glm::ivec3 block_pos, let::bridge::render_data::block_face face) {
    const auto offset = glm::vec3(chunk.x * 16 + block_pos.x, block_pos.y, chunk.z * 16 + block_pos.z);
    const auto cube_vertices = std::array<glm::vec3, 8>({
            glm::vec3(0, 0, 0) + offset,
            glm::vec3(1, 0, 0) + offset,
            glm::vec3(1, 1, 0) + offset,
            glm::vec3(0, 1, 0) + offset,
            glm::vec3(0, 0, 1) + offset,
            glm::vec3(1, 0, 1) + offset,
            glm::vec3(1, 1, 1) + offset,
            glm::vec3(0, 1, 1) + offset
        });

    auto normal = glm::vec3();
    auto indices = std::array<std::uint32_t, 6>();
    switch (face)
    {
        case block_face::north:
            indices = { 0, 1, 2, 2, 3, 0 };
            normal = glm::vec3(0, 0, 1);
            break;
        case block_face::east:
            indices = { 1, 5, 6, 6, 2, 1 };
            normal = glm::vec3(1, 0, 0);
            break;
        case block_face::south:
            indices = { 7, 6, 5, 4, 4, 7 };
            normal = glm::vec3(0, 0, -1);
            break;
        case block_face::west:
            indices = { 4, 0, 3, 3, 7, 4 };
            normal = glm::vec3(-1, 0, 0);
            break;
        case block_face::top:
            indices = { 3, 2, 6, 6, 7, 3 };
            normal = glm::vec3(0, 1, 0);
            break;
        case block_face::bottom:
            indices = { 4, 5, 1, 1, 0, 4 };
            normal = glm::vec3(0, -1, 0);
            break;
    }

    for (auto index : indices) {
        _indices.push_back(index + _vertices.size());
    }

    const auto col = glm::vec3(0.2, 0.4, 0.8);

    for (auto vert : cube_vertices)
    {
        _vertices.push_back(vertex{
            .pos = vert,
            .col = col,
            .normal = normal
        });
    }

}
