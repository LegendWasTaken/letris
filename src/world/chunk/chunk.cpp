//
// Created by howto on 6/9/2021.
//

#include "chunk.h"

std::uint16_t let::world::chunk_section::get_block(std::int32_t x, std::int32_t y, std::int32_t z) const noexcept {
    return blocks[(y & 0xF) << 8 | (z & 0xF) << 4 | (x & 0xF)];
}

void let::world::chunk_section::set_block(std::int32_t x, std::int32_t y, std::int32_t z, std::uint16_t block) noexcept {
    blocks[(y << 8) | (z << 4) | x] = block;
}

void let::world::chunk::_ensure_chunk(std::uint8_t y) {
    sections[y] = std::make_unique<chunk_section>();
}

let::world::chunk::chunk(std::int32_t x, std::int32_t z) : x(x), z(z) {
    for (auto &ptr : sections)
        ptr = nullptr;
}

std::uint16_t let::world::chunk::get_block(std::int32_t x, std::int32_t y, std::int32_t z) const noexcept {
    if (auto &ptr = sections[y >> 4]; ptr != nullptr)
        return ptr->get_block(x & 0xF, y & 0xF, z & 0xF);
    return 0; // Replace to air once i get all of that sorted
}

void let::world::chunk::set_block(std::int32_t x, std::int32_t y, std::int32_t z, std::uint16_t block) noexcept {
    _ensure_chunk(y >> 4);
    sections[y >> 4]->set_block(x & 0xF, y & 0xF, z & 0xF, block);
}

let::world::chunk_section *let::world::chunk::operator[](size_t y) noexcept {
    return sections[y].get();
}

const let::world::chunk_section *let::world::chunk::operator[](size_t y) const noexcept {
    return sections[y].get();
}
