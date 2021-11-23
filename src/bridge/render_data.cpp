#include "render_data.h"

let::bridge::render_data::render_data(const let::world &world) {
    ZoneScopedN("render_data::render_data");

    for (const auto &it : world._chunks) {
        const auto &chunk = it.second;

        for (auto x = 0; x < 16; x++)
            for (auto y = 0; y < 256; y++)
                for (auto z = 0; z < 16; z++)
                {
                    if (chunk.get_block(x, y, z))
                    {
                        auto pos = glm::ivec3(x, y, z);

                        auto surrounding = std::bitset<6>();

                        if (pos.z == 0 || !chunk.get_block(x, y, z - 1))
                            surrounding.set(0);

                        if (pos.x == 15 || !chunk.get_block(x + 1, y, z))
                            surrounding.set(1);

                        if (pos.z == 15 || !chunk.get_block(x, y, z + 1))
                            surrounding.set(2);

                        if (pos.x == 0 || !chunk.get_block(x - 1, y, z))
                            surrounding.set(3);

                        if (pos.y == 255 || !chunk.get_block(x, y + 1, z))
                            surrounding.set(4);

                        if (pos.y == 0 || !chunk.get_block(x, y - 1, z))
                            surrounding.set(5);


                        for (auto i = 0; i < surrounding.size(); i++)
                            if (surrounding[i])
                                _add_face({chunk.x, chunk.z}, pos, static_cast<block_face>(i));

                    }
                }

        break;
    }
}

std::span<const let::renderer::vertex> let::bridge::render_data::vertices() const noexcept {
    return _vertices;
}

std::span<const std::uint32_t> let::bridge::render_data::indices() const noexcept {
    return _indices;
}

void
let::bridge::render_data::_add_face(glm::ivec2 chunk, glm::ivec3 block_pos, let::bridge::render_data::block_face face) {
    const auto offset = glm::vec3(chunk.x * 16 + block_pos.x, block_pos.y, chunk.y * 16 + block_pos.z);
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
            indices = { 0, 2, 1, 2, 0, 3 };
            normal = glm::vec3(0, 0, -1);
            break;
        case block_face::east:
            indices = { 1, 6, 5, 6, 1, 2 };
            normal = glm::vec3(1, 0, 0);
            break;
        case block_face::south:
            indices = { 7, 5, 6, 5, 7, 4 };
            normal = glm::vec3(0, 0, 1);
            break;
        case block_face::west:
            indices = { 4, 3, 0, 3, 4, 7 };
            normal = glm::vec3(-1, 0, 0);
            break;
        case block_face::top:
            indices = { 3, 6, 2, 6, 3, 7 };
            normal = glm::vec3(0, 1, 0);
            break;
        case block_face::bottom:
            indices = { 4, 1, 5, 1, 4, 0 };
            normal = glm::vec3(0, -1, 0);
            break;
    }

    for (auto index : indices) {
        _indices.push_back(index + _vertices.size());
    }

    const auto col = glm::vec3(1.0, 1.0, 1.0);

    for (auto vert : cube_vertices)
    {
        _vertices.push_back(renderer::vertex{
                .pos = vert,
                .col = col,
                .normal = normal
        });
    }

}
