#pragma once

#include <cstdint>
#include <string_view>

#include <glm/glm.hpp>

namespace let
{
  class angle
  {
  public:
    std::uint8_t val;

    explicit angle(std::byte val) : val(static_cast<uint8_t>(val))
    {}

    operator float() const noexcept
    {
      return static_cast<float>(val) * (360.0f / 256.0f);
    }
  };

  class var_int
  {
  public:
    std::int32_t val;

    explicit var_int(std::int32_t val) : val(val)
    {}

    operator std::int32_t() const noexcept
    {
      return val;
    }
  };

  class var_long
  {
  public:
    std::int64_t val;

    explicit var_long(std::int64_t val) : val(val)
    {}

    operator std::int64_t() const noexcept
    {
      return val;
    }
  };

  class UUID final
  {
  public:
    static UUID from_string(std::string_view string);

    static UUID invalid()
    { return UUID(0, 0); }

    UUID(
            uint64_t most_sig,
            uint64_t least_sig) noexcept;

    [[nodiscard]] bool operator==(const UUID &other) noexcept;

    [[nodiscard]] uint64_t most_significant_bits() const noexcept;

    [[nodiscard]] uint64_t least_significant_bits() const noexcept;

    [[nodiscard]] std::string to_string() const;

    bool valid() const noexcept;

  private:
    uint64_t _most_sig_bits;
    uint64_t _least_sig_bits;
  };
}