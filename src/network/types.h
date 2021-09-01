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

    var_int() = default;

    explicit var_int(std::int32_t val) : val(val)
    {}

    explicit operator std::int32_t() const noexcept
    {
      return val;
    }

    [[nodiscard]] std::int32_t length() const noexcept
    {
        auto u_value = std::uint32_t(static_cast<std::int32_t>(val));

        auto times = 0;
        do
        {
            auto temp = static_cast<std::uint8_t>(u_value & 0b01111111);
            u_value >>= 7;

            if (u_value != 0)
                temp |= 0b10000000;
            times++;
        } while (u_value != 0);
        return times;
    }

  };

  class var_long
  {
  public:
    std::int64_t val;

    var_long() = default;

    explicit var_long(std::int64_t val) : val(val)
    {}

    operator std::int64_t() const noexcept
    {
      return val;
    }

      [[nodiscard]] std::int32_t length() const noexcept
      {
          auto u_value = std::uint32_t(static_cast<std::int32_t>(val));

          auto times = 0;
          do
          {
              auto temp = static_cast<std::uint8_t>(u_value & 0b01111111);
              u_value >>= 7;

              if (u_value != 0)
                  temp |= 0b10000000;
              times++;
          } while (u_value != 0);
          return times;
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