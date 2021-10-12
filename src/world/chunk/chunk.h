#pragma once

#include <cstdint>
#include <array>
#include <memory>

namespace let {

    class chunk_section {
    public:
        [[nodiscard]] std::uint16_t get_block(std::int32_t x, std::int32_t y, std::int32_t z) const noexcept;
        void set_block(std::int32_t x, std::int32_t y, std::int32_t z, std::uint16_t) noexcept;

        chunk_section() = default;

        chunk_section(const chunk_section &) = delete;
        chunk_section(chunk_section &&) = delete;

        chunk_section& operator =(const chunk_section &) = delete;
        chunk_section& operator =(chunk_section &&) = delete;

        std::array<std::uint16_t, 16 * 16 * 16> blocks;
    };

    class chunk {
    private:
        std::array<std::unique_ptr<chunk_section>, 16> sections;

        void _ensure_chunk(std::uint8_t y);

    public:
        const std::int32_t x;
        const std::int32_t z;

        chunk(std::int32_t x, std::int32_t z);

        [[nodiscard]] std::uint16_t get_block(std::int32_t x, std::int32_t y, std::int32_t z) const noexcept;
        void set_block(std::int32_t x, std::int32_t y, std::int32_t z, std::uint16_t block) noexcept;

        [[nodiscard]] chunk_section *operator[](size_t y) noexcept;
        [[nodiscard]] const chunk_section *operator[](size_t y) const noexcept;

    };
}
