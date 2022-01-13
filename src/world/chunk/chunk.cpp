#include "chunk.h"

size_t let::chunk_section::visible_faces() const noexcept {
    auto faces = size_t(0);
    for (auto block : blocks)
        if (block.id() != 0)
            for (int i = 0; i < 6; i++)
                faces += block.visible(static_cast<block::face>(i));
    return faces;
}

void let::chunk::_ensure_chunk(std::uint8_t y) {
    if (sections[y] == nullptr)
        sections[y] = std::make_unique<chunk_section>();
}

let::chunk::chunk(std::int32_t x, std::int32_t z) : x(x), z(z) {
    for (auto &ptr : sections)
        ptr = nullptr;
}

let::chunk_section *let::chunk::operator[](size_t y) noexcept {
    return sections[y].get();
}

const let::chunk_section *let::chunk::operator[](size_t y) const noexcept {
    return sections[y].get();
}

std::unique_ptr<let::chunk_section> &let::chunk::section_at(size_t y) {
    return sections[y];
}

std::uint64_t let::chunk::key(const let::chunk &chunk) noexcept {
    return
            ((static_cast<std::uint64_t>(chunk.x) << 32) & 0xFFFFFFFF00000000) |
            ((static_cast<std::uint64_t>(chunk.z) <<  0) & 0x00000000FFFFFFFF);
}

std::uint64_t let::chunk::key(std::int32_t x, std::int32_t z) noexcept {
    return
            ((static_cast<std::uint64_t>(x) << 32) & 0xFFFFFFFF00000000) |
            ((static_cast<std::uint64_t>(z) <<  0) & 0x00000000FFFFFFFF);
}

glm::ivec2 let::chunk::decompose_key(std::uint64_t key) noexcept {
    auto x = key >> 32;
    auto z = key & 0xFFFFFFFF;
    return glm::ivec2(x, z);
}
