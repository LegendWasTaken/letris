#pragma once

#include <network/byte_buffer.h>
#include <network/types.h>

#include <string>
#include <stdexcept>

namespace let::network::decoder
{
  inline void read(let::network::byte_buffer &buffer, bool &value)
  {
    value = !static_cast<bool>(buffer.next_byte());
  }

  inline void read(let::network::byte_buffer &buffer, std::byte &value)
  {
    value = buffer.next_byte();
  }

  inline void read(let::network::byte_buffer &buffer, std::uint8_t &value)
  {
    value = static_cast<uint8_t>(buffer.next_byte());
  }

  inline void read(let::network::byte_buffer &buffer, std::uint16_t &value)
  {
    value = (static_cast<std::uint16_t>(buffer.next_byte()) << 8) | (static_cast<std::uint16_t>(buffer.next_byte()));
  }

  inline void read(let::network::byte_buffer &buffer, std::int16_t &value)
  {
    value = static_cast<std::int16_t>((static_cast<std::uint16_t>(buffer.next_byte()) << 8) |
                                      (static_cast<std::uint16_t>(buffer.next_byte())));
  }

  inline void read(let::network::byte_buffer &buffer, std::uint32_t &value)
  {
    value = (static_cast<std::uint32_t>(buffer.next_byte()) << 24) | (static_cast<std::uint32_t>(buffer.next_byte()) << 16) |
            (static_cast<std::uint32_t>(buffer.next_byte()) << 8) | (static_cast<std::uint32_t>(buffer.next_byte()) << 0);
  }

  inline void read(let::network::byte_buffer &buffer, std::int32_t &value)
  {
    auto unsigned_int = uint32_t();
    read(buffer, unsigned_int);
    value = unsigned_int;
  }

  inline void read(let::network::byte_buffer &buffer, std::uint64_t &value)
  {
    value = (static_cast<std::uint64_t>(buffer.next_byte()) << 56) | (static_cast<std::uint64_t>(buffer.next_byte()) << 48) |
            (static_cast<std::uint64_t>(buffer.next_byte()) << 40) | (static_cast<std::uint64_t>(buffer.next_byte()) << 32) |
            (static_cast<std::uint64_t>(buffer.next_byte()) << 24) | (static_cast<std::uint64_t>(buffer.next_byte()) << 16) |
            (static_cast<std::uint64_t>(buffer.next_byte()) << 8) | (static_cast<std::uint64_t>(buffer.next_byte()) << 0);
  }

  inline void read(let::network::byte_buffer &buffer, std::int64_t &value)
  {
    auto unsigned_long = uint64_t();
    read(buffer, unsigned_long);
    value = unsigned_long;
  }

  inline void read(let::network::byte_buffer &buffer, float &value)
  {
    auto int_val = uint32_t();
    read(buffer, int_val);
    auto float_value = 0.0f;
    std::memcpy(&float_value, &int_val, 4);
    value = float_value;
  }

  inline void read(let::network::byte_buffer &buffer, double &value)
  {
    auto int_val = uint64_t();
    read(buffer, int_val);
    auto double_value = 0.0;
    std::memcpy(&double_value, &int_val, 4);
    value = double_value;
  }

  inline void read(let::network::byte_buffer &buffer, let::var_int &value)
  {
    // Directly from wiki.vg
    auto num_read = std::int32_t(0);
    auto result = std::int32_t(0);
    auto read = std::uint8_t (0);
    do {
      read      = static_cast<std::uint8_t>(buffer.next_byte());
      auto val = (read & 0b01111111);
      result |= (val << (7 * num_read++));
      if (num_read > 5) { throw std::runtime_error("Read var-int that's too large!"); }
    } while ((read & 0b10000000) != 0);

    value = var_int(result);
  }

  inline void read(let::network::byte_buffer &buffer, let::var_long &value)
  {
    // Directly from wiki.vg
    auto num_read = std::int32_t(0);
    auto result = std::int32_t(0);
    auto read = std::uint8_t (0);
    do {
      read      = static_cast<std::uint8_t>(buffer.next_byte());
      auto val = (read & 0b01111111);
      result |= (val << (7 * num_read++));
      if (num_read > 10) { throw std::runtime_error("Read var-long that's too large!"); }
    } while ((read & 0b10000000) != 0);

    value = var_long(result);
  }

  inline void read(let::network::byte_buffer &buffer, std::string &value)
  {
    auto var = let::var_int(0);
    read(buffer, var);
    auto length = static_cast<std::int32_t>(var);
    if (length < 0 || length > 32767) throw std::runtime_error("String size is impossible");

    auto data = buffer._next_bytes(length);

    auto string = std::string();
    string.resize(length, '\0');
    for (auto i = 0; i < length; i++)
      string[i] = static_cast<char>(data[i]);

    value = std::move(string);
  }

  inline void read(let::network::byte_buffer &buffer, let::UUID &value)
  {
    auto lhs = uint64_t();
    read(buffer, lhs);
    auto rhs = uint64_t();
    read(buffer, rhs);
    value = UUID(lhs, rhs);
  }

  inline void read(let::network::byte_buffer &buffer, glm::ivec3 &value)
  {
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

  inline void read(let::network::byte_buffer &buffer, let::angle &value)
  {
    auto angle = std::byte();
    read(buffer, angle);
    value = let::angle(angle);
  }
}
