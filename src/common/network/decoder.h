#pragma once

#include <common/network/byte_buffer.h>
#include <common/network/types.h>
#include <common/exception.h>

#include <string>
#include <stdexcept>

namespace let::network::decoder {
    inline void read(let::network::byte_buffer &buffer, bool &value) {
        value = static_cast<bool>(buffer.next_byte());
    }

    inline void read(let::network::byte_buffer &buffer, std::byte &value) {
        value = buffer.next_byte();
    }

    inline void read(let::network::byte_buffer &buffer, std::uint8_t &value) {
        value = static_cast<uint8_t>(buffer.next_byte());
    }

    inline void read(let::network::byte_buffer &buffer, std::uint16_t &value) {
        value = (static_cast<std::uint16_t>(buffer.next_byte()) << 8) |
                (static_cast<std::uint16_t>(buffer.next_byte()));
    }

    inline void read(let::network::byte_buffer &buffer, std::int16_t &value) {
        value = static_cast<std::int16_t>((static_cast<std::uint16_t>(buffer.next_byte()) << 8) |
                                          (static_cast<std::uint16_t>(buffer.next_byte())));
    }

    inline void read(let::network::byte_buffer &buffer, std::uint32_t &value) {
        value = (static_cast<std::uint32_t>(buffer.next_byte()) << 24) |
                (static_cast<std::uint32_t>(buffer.next_byte()) << 16) |
                (static_cast<std::uint32_t>(buffer.next_byte()) << 8) |
                (static_cast<std::uint32_t>(buffer.next_byte()) << 0);
    }

    inline void read(let::network::byte_buffer &buffer, std::int32_t &value) {
        auto unsigned_int = uint32_t();
        read(buffer, unsigned_int);
        value = unsigned_int;
    }

    inline void read(let::network::byte_buffer &buffer, std::uint64_t &value) {
        value = (static_cast<std::uint64_t>(buffer.next_byte()) << 56) |
                (static_cast<std::uint64_t>(buffer.next_byte()) << 48) |
                (static_cast<std::uint64_t>(buffer.next_byte()) << 40) |
                (static_cast<std::uint64_t>(buffer.next_byte()) << 32) |
                (static_cast<std::uint64_t>(buffer.next_byte()) << 24) |
                (static_cast<std::uint64_t>(buffer.next_byte()) << 16) |
                (static_cast<std::uint64_t>(buffer.next_byte()) << 8) |
                (static_cast<std::uint64_t>(buffer.next_byte()) << 0);
    }

    inline void read(let::network::byte_buffer &buffer, std::int64_t &value) {
        auto unsigned_long = uint64_t();
        read(buffer, unsigned_long);
        value = unsigned_long;
    }

    inline void read(let::network::byte_buffer &buffer, float &value) {
        auto int_val = uint32_t();
        read(buffer, int_val);
        auto float_value = 0.0f;
        std::memcpy(&float_value, &int_val, 4);
        value = float_value;
    }

    inline void read(let::network::byte_buffer &buffer, double &value) {
        auto int_val = uint64_t();
        read(buffer, int_val);
        auto double_value = 0.0;
        std::memcpy(&double_value, &int_val, 8);
        value = double_value;
    }

    inline void read(let::network::byte_buffer &buffer, let::var_int &value) {
        int numRead = 0;
        int32_t result  = 0;
        uint8_t read;
        do {
            read       = static_cast<uint8_t>(buffer.next_byte());
            long val = (read & 0b01111111);
            result |= (val << (7 * numRead));

            numRead++;
            if (numRead > 5)
                LET_EXCEPTION(exception::source_type::network, "VarInt too large");
        } while ((read & 0b10000000) != 0);

        value.val = result;
    }

    inline void read(let::network::byte_buffer &buffer, let::var_long &value) {
        int numRead = 0;
        uint64_t result  = 0;
        uint8_t read;
        do {
            read       = static_cast<uint8_t>(buffer.next_byte());
            uint64_t val = (read & 0b01111111);
            result |= (val << (7 * numRead));

            numRead++;
            if (numRead > 10)
                LET_EXCEPTION(exception::source_type::network, "Varlong too large");
        } while ((read & 0b10000000) != 0);

        value.val = result;
    }

    inline void read(let::network::byte_buffer &buffer, std::string &value) {
        auto var = let::var_int(0);
        read(buffer, var);
        auto length = static_cast<std::int32_t>(var);
        if (length < 0 || length > 32767) LET_EXCEPTION(exception::source_type::network, "String too large");

        auto data = buffer.next_bytes(length);

        auto string = std::string();
        string.resize(length, '\0');
        for (auto i = 0; i < length; i++)
            string[i] = static_cast<char>(data[i]);

        value = std::move(string);
    }

    inline void read(let::network::byte_buffer &buffer, let::uuid &value) {
        auto lhs = uint64_t();
        read(buffer, lhs);
        auto rhs = uint64_t();
        read(buffer, rhs);
        value = uuid(lhs, rhs);
    }

    inline void read(let::network::byte_buffer &buffer, glm::ivec3 &value) {
        auto packed = uint64_t();
        read(buffer, packed);
        auto x = static_cast<std::int32_t>(packed >> 38);
        auto y = static_cast<std::int32_t>((packed >> 26) & 0xFFF);
        auto z = static_cast<std::int32_t>(packed << 38 >> 38);

        if (x > (1 << 25)) x -= 1 << 26;
        if (y > (1 << 11)) y -= 1 << 12;
        if (z > (1 << 25)) z -= 1 << 26;

        value = glm::ivec3(x, y, z);
    }

    inline void read(let::network::byte_buffer &buffer, let::angle &value) {
        auto angle = std::byte();
        read(buffer, angle);
        value = let::angle(angle);
    }

    inline void read(let::network::byte_buffer &buffer, let::chat &chat) {
        auto str = std::string();
        read(buffer, str);
        chat = let::chat(str);
    }

    inline void read(let::network::byte_buffer &buffer, let::slot &t_slot) {
        auto block_data = std::int16_t();
        read(buffer, block_data);
        if (block_data != -1)
        {
            auto item_count = std::uint8_t();
            auto damage = std::int16_t();
            read(buffer, item_count);
            read(buffer, damage);

            LET_EXCEPTION(exception::source_type::network, "Untested, test this codepath");
            if (damage != 0xFFFF) {
                t_slot.nbt_data = nbt::node::read(buffer);
            }

            t_slot.item_id = block_data;
            t_slot.item_count = item_count;
        }
    }

    inline void read(let::network::byte_buffer &buffer, let::entity_metadata &metadata) {
        do {
            auto item = std::byte();
            read(buffer, item);
            if (item == std::byte(0x7F))
                break;

            auto index = item & std::byte(0x1F);
            auto type = std::int32_t(item >> 5);

            if (type == 0) {
                auto value = std::byte();
                read(buffer, value);
                metadata.data.insert({index, value});
            }
            if (type == 1) {
                auto value = std::int16_t();
                read(buffer, value);
                metadata.data.insert({index, value});
            }
            if (type == 2) {
                auto value = std::int32_t();
                read(buffer, value);
                metadata.data.insert({index, value});
            }
            if (type == 3) {
                auto value = float();
                read(buffer, value);
                metadata.data.insert({index, value});
            }
            if (type == 4) {
                auto value = std::string();
                read(buffer, value);
                metadata.data.insert({index, value});
            }
            if (type == 5) {
                auto value = let::slot();
                read(buffer, value);
                metadata.data.insert({index, value});
            }
            if (type == 8) {
                auto value = glm::ivec3();
                read(buffer, value.x);
                read(buffer, value.y);
                read(buffer, value.z);
                metadata.data.insert({index, value});
            }
            if (type == 7) {
                auto value = glm::vec3();
                read(buffer, value.x);
                read(buffer, value.y);
                read(buffer, value.z);
                metadata.data.insert({index, value});
            }

        } while (true);
    }
}
