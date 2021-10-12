#pragma once

#include <common/network/byte_buffer.h>
#include <common/network/types.h>

#include <string>
#include <stdexcept>

namespace let::network::encoder {
    inline void write(let::network::byte_buffer &buffer, bool value) {
        buffer.write_byte(static_cast<std::byte>(value));
    }

    inline void write(let::network::byte_buffer &buffer, std::byte value) {
        buffer.write_byte(value);
    }

    inline void write(let::network::byte_buffer &buffer, std::uint8_t value) {
        buffer.write_byte(static_cast<std::byte>(value));
    }

    inline void write(let::network::byte_buffer &buffer, std::uint16_t value) {
        buffer.write_byte(std::byte((value >> 8) & 0xFF));
        buffer.write_byte(std::byte((value >> 0) & 0xFF));
    }

    inline void write(let::network::byte_buffer &buffer, std::int16_t value) {
        buffer.write_byte(std::byte((value >> 8) & 0xFF));
        buffer.write_byte(std::byte((value >> 0) & 0xFF));
    }

    inline void write(let::network::byte_buffer &buffer, std::uint32_t value) {
        buffer.write_byte(std::byte((value >> 24) & 0xFF));
        buffer.write_byte(std::byte((value >> 16) & 0xFF));
        buffer.write_byte(std::byte((value >> 8) & 0xFF));
        buffer.write_byte(std::byte((value >> 0) & 0xFF));
    }

    inline void write(let::network::byte_buffer &buffer, std::int32_t value) {
        buffer.write_byte(std::byte((value >> 24) & 0xFF));
        buffer.write_byte(std::byte((value >> 16) & 0xFF));
        buffer.write_byte(std::byte((value >> 8) & 0xFF));
        buffer.write_byte(std::byte((value >> 0) & 0xFF));
    }

    inline void write(let::network::byte_buffer &buffer, std::uint64_t value) {
        buffer.write_byte(std::byte((value >> 56) & 0xFF));
        buffer.write_byte(std::byte((value >> 48) & 0xFF));
        buffer.write_byte(std::byte((value >> 40) & 0xFF));
        buffer.write_byte(std::byte((value >> 32) & 0xFF));
        buffer.write_byte(std::byte((value >> 24) & 0xFF));
        buffer.write_byte(std::byte((value >> 16) & 0xFF));
        buffer.write_byte(std::byte((value >> 8) & 0xFF));
        buffer.write_byte(std::byte((value >> 0) & 0xFF));
    }

    inline void write(let::network::byte_buffer &buffer, std::int64_t value) {
        buffer.write_byte(std::byte((value >> 56) & 0xFF));
        buffer.write_byte(std::byte((value >> 48) & 0xFF));
        buffer.write_byte(std::byte((value >> 40) & 0xFF));
        buffer.write_byte(std::byte((value >> 32) & 0xFF));
        buffer.write_byte(std::byte((value >> 24) & 0xFF));
        buffer.write_byte(std::byte((value >> 16) & 0xFF));
        buffer.write_byte(std::byte((value >> 8) & 0xFF));
        buffer.write_byte(std::byte((value >> 0) & 0xFF));
    }

    inline void write(let::network::byte_buffer &buffer, float value) {
        auto as_int = std::uint32_t();
        std::memcpy(&as_int, &value, 4);
        write(buffer, as_int);
    }

    inline void write(let::network::byte_buffer &buffer, double value) {
        auto as_int = std::uint64_t();
        std::memcpy(&as_int, &value, 8);
        write(buffer, as_int);
    }

    inline void write(let::network::byte_buffer &buffer, let::var_int value) {
        int written = 0;
        auto u_value = static_cast<uint32_t>(value.val);
        do {
            auto temp = static_cast<uint8_t>(u_value & 0b01111111);
            u_value >>= 7;
            if (u_value != 0) { temp |= 0b10000000; }
            buffer.write_byte(std::byte(temp));
            written += 1;
        } while (u_value != 0);
    }

    inline void write(let::network::byte_buffer &buffer, let::var_long value) {
        auto u_value = static_cast<uint64_t>(value.val);
        do {
            auto temp = static_cast<uint8_t>(u_value & 0b01111111);
            u_value >>= 7;
            if (u_value != 0) { temp |= 0b10000000; }
            buffer.write_byte(std::byte(temp));
        } while (u_value != 0);
    }

    inline void write(let::network::byte_buffer &buffer, const std::string &value) {
        write(buffer, let::var_int(value.length()));
        for (const auto &element : value)
            write(buffer, static_cast<std::uint8_t>(element));
    }

    inline void write(let::network::byte_buffer &buffer, let::uuid value) {
        write(buffer, value.most_significant_bits());
        write(buffer, value.least_significant_bits());
    }

    inline void write(let::network::byte_buffer &buffer, glm::ivec3 value) {
        const auto packed = std::uint64_t((static_cast<std::uint64_t>(value.x) & 0x3FFFFFF) << 38 |
                                          (static_cast<std::uint64_t>(value.y) & 0xFFF) << 26 |
                                          (static_cast<std::uint64_t>(value.z) & 0x3FFFFFF) << 0);

        write(buffer, packed);
    }

    inline void write(let::network::byte_buffer &buffer, let::angle value) {
        const auto packed = std::byte(float(value) * (256.f / 360.f));
        write(buffer, packed);
    }
}