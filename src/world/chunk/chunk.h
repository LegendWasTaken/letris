#pragma once

#include <cstdint>
#include <array>
#include <memory>

#include <world/block.h>

#include <tracy/Tracy.hpp>

namespace let {

    class chunk_section {
    public:

        [[nodiscard]] block get_block(std::int32_t x, std::int32_t y, std::int32_t z) const noexcept {
            return blocks[(y & 0xF) << 8 | (z & 0xF) << 4 | (x & 0xF)];
        }
        void set_block(std::int32_t x, std::int32_t y, std::int32_t z, block b) noexcept {
            blocks[(y << 8) | (z << 4) | x] = b;
        }
        [[nodiscard]] size_t visible_faces() const noexcept;

        chunk_section() = default;

        mutable bool should_rerender = false;
        std::array<let::block, 16 * 16 * 16> blocks;
    };

    class chunk {
    private:
        std::array<std::unique_ptr<chunk_section>, 16> sections;

        void _ensure_chunk(std::uint8_t y);

    public:
        [[nodiscard]] static std::uint64_t key(const chunk &chunk) noexcept;

        [[nodiscard]] static std::uint64_t key(std::int32_t x, std::int32_t z) noexcept;

        // Todo: Make this private and make a method in the world that allows for cached block updates
        mutable bool should_rerender{};

        const std::int32_t x;

        const std::int32_t z;

        chunk(std::int32_t x, std::int32_t z);

        [[nodiscard]] block get_block(std::int32_t bx, std::int32_t by, std::int32_t bz) const noexcept {
            if (auto &ptr = sections[by >> 4]; ptr != nullptr)
                return ptr->get_block(bx & 0xF, by & 0xF, bz & 0xF);
            return block(0); // Replace to air once i get all of that sorted
        }

        void set_block(std::int32_t bx, std::int32_t by, std::int32_t bz, block b) noexcept {
            _ensure_chunk(by >> 4);
            sections[by >> 4]->set_block(bx & 0xF, by & 0xF, bz & 0xF, b);
        }

        bool rerender_any() const noexcept {
            auto should = should_rerender;
            for (auto &section : sections)
            {
                if (section != nullptr)
                    should |= section->should_rerender;
            }
            return should;
        }

        void rerendered() const noexcept {
            should_rerender = false;
            for (auto &section : sections)
            {
                if (section != nullptr)
                    section->should_rerender = false;
            }
        }

        [[nodiscard]] chunk_section *operator[](size_t y) noexcept;

        [[nodiscard]] const chunk_section *operator[](size_t y) const noexcept;

        [[nodiscard]] std::unique_ptr<chunk_section> &section_at(size_t y);
    };
}
