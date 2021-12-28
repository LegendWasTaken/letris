#pragma once

#include <cstdint>
#include <array>
#include <memory>

#include <world/block.h>

#include <tracy/Tracy.hpp>

namespace let {

    class chunk_section {
    public:

        [[nodiscard]] std::uint16_t get_block(std::int32_t x, std::int32_t y, std::int32_t z) const noexcept {
            return blocks[(y & 0xF) << 8 | (z & 0xF) << 4 | (x & 0xF)].id();
        }
        void set_block(std::int32_t x, std::int32_t y, std::int32_t z, std::uint16_t block) noexcept {
            blocks[(y << 8) | (z << 4) | x].set_id(block);
        }

        chunk_section() = default;

        std::array<let::block, 16 * 16 * 16> blocks;
    };

    class chunk {
    private:
        std::array<std::unique_ptr<chunk_section>, 16> sections;

        void _ensure_chunk(std::uint8_t y);

    public:
        [[nodiscard]] static std::uint64_t key(const chunk &chunk) noexcept;

        [[nodiscard]] static std::uint64_t key(std::int32_t x, std::int32_t z) noexcept;

        const std::int32_t x;
        const std::int32_t z;

        chunk(std::int32_t x, std::int32_t z);

        [[nodiscard]] std::uint16_t get_block(std::int32_t x, std::int32_t y, std::int32_t z) const noexcept {
            if (auto &ptr = sections[y >> 4]; ptr != nullptr)
                return ptr->get_block(x & 0xF, y & 0xF, z & 0xF);
            return 0; // Replace to air once i get all of that sorted
        }

        void set_block(std::int32_t x, std::int32_t y, std::int32_t z, std::uint16_t block) noexcept {
            _ensure_chunk(y >> 4);
            sections[y >> 4]->set_block(x & 0xF, y & 0xF, z & 0xF, block);
        }

        [[nodiscard]] chunk_section *operator[](size_t y) noexcept;
        [[nodiscard]] const chunk_section *operator[](size_t y) const noexcept;
        [[nodiscard]] std::unique_ptr<chunk_section> &section_at(size_t y);
        [[nodiscard]] size_t visible_faces() const noexcept;
    };
}
