#pragma once

#include "types.h"

namespace
{
  // Hex to decimal
  [[nodiscard]] std::int8_t htod(std::int8_t c)
  {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;    // Not a hex digit
  }

  [[nodiscard]] std::int8_t decimal_to_hex(std::uint32_t decimal)
  {
    if (decimal >= 0 && decimal <= 15) return "0123456789abcdef"[decimal];
    return '?';
  }

  void u8_to_hex(std::uint8_t x, std::int8_t *buf)
  {
    buf[0] = decimal_to_hex(static_cast<std::int32_t>(x) >> 4);
    buf[1] = decimal_to_hex(static_cast<std::int32_t>(x) & 0xF);
  }

  void u_long_to_hex(std::uint64_t x, std::int8_t *buf)
  {
    for (auto i = 0; i < 8; ++i)
    {
      u8_to_hex(x & 0xFF, buf + 2 * i);
      x >>= 8;
    }
  }

}

let::UUID let::UUID::from_string(std::string_view string)
{
  if (string.length() != 36) return let::UUID::invalid();

  if (string[8] != '-' || string[13] != '-' || string[18] != '-' || string[23] != '-')
    return let::UUID::invalid();

  std::int8_t bytes[16];

  std::int32_t byte_index = 0;
  std::int32_t hyphen_count = 0;
  for (std::int32_t i = 0; i < 36; i += 2)
  {
    std::int32_t hyphens = (string[i] == '-') + (string[i + 1] == '-');
    if (hyphens != 0)
    {
      hyphen_count += hyphens;
      i -= 1;
      continue;
    }

    const std::int8_t most_sig_bits = htod(string[i]);
    const std::int8_t least_sig_bits = htod(string[i + 1]);
    if (most_sig_bits == -1 || least_sig_bits == -1) return let::UUID::invalid();

    bytes[byte_index++] = (most_sig_bits << 4) | least_sig_bits;
  }

  if (hyphen_count != 4) return UUID::invalid();

  std::uint64_t most_sig_bits, least_sig_bits;
  std::memcpy(&most_sig_bits, bytes, 8);
  std::memcpy(&least_sig_bits, bytes + 8, 8);

  return UUID(most_sig_bits, least_sig_bits);
}

let::UUID::UUID(
        uint64_t most_sig,
        uint64_t least_sig) noexcept: _most_sig_bits(most_sig), _least_sig_bits(least_sig)
{
}

bool let::UUID::operator==(const let::UUID &other) noexcept
{
  return _most_sig_bits == other._most_sig_bits && _least_sig_bits == other._least_sig_bits;
}

uint64_t let::UUID::most_significant_bits() const noexcept
{
  return _most_sig_bits;
}

uint64_t let::UUID::least_significant_bits() const noexcept
{
  return _least_sig_bits;
}

std::string let::UUID::to_string() const
{
  std::int8_t hex[32];
  u_long_to_hex(_least_sig_bits, hex);
  u_long_to_hex(_most_sig_bits, hex + 16);

  std::string result(36, '-');
  std::memcpy(&result[0], &hex[0], 8);
  std::memcpy(&result[9], &hex[8], 4);
  std::memcpy(&result[14], &hex[12], 4);
  std::memcpy(&result[19], &hex[16], 4);
  std::memcpy(&result[24], &hex[20], 12);

  return result;
}

bool let::UUID::valid() const noexcept
{
  return _most_sig_bits != 0 && _least_sig_bits != 0;
}




