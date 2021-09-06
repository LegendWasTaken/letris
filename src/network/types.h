#pragma once

#include <cstdint>
#include <string_view>
#include <variant>

#include <network/nbt.h>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

namespace let {
    class angle {
    public:
        std::uint8_t val;

        angle() = default;

        explicit angle(std::byte val) : val(static_cast<uint8_t>(val)) {}

        explicit operator float() const noexcept {
            return static_cast<float>(val) * (360.0f / 256.0f);
        }
    };

    class var_int {
    public:
        std::int32_t val;

        var_int() = default;

        explicit var_int(std::int32_t val) : val(val) {}

        explicit operator std::int32_t() const noexcept {
            return val;
        }

        [[nodiscard]] std::int32_t length() const noexcept {
            auto u_value = std::uint32_t(static_cast<std::int32_t>(val));

            auto times = 0;
            do {
                auto temp = static_cast<std::uint8_t>(u_value & 0b01111111);
                u_value >>= 7;

                if (u_value != 0)
                    temp |= 0b10000000;
                times++;
            } while (u_value != 0);
            return times;
        }

    };

    class var_long {
    public:
        std::int64_t val;

        var_long() = default;

        explicit var_long(std::int64_t val) : val(val) {}

        operator std::int64_t() const noexcept {
            return val;
        }

        [[nodiscard]] std::int32_t length() const noexcept {
            auto u_value = std::uint32_t(static_cast<std::int32_t>(val));

            auto times = 0;
            do {
                auto temp = static_cast<std::uint8_t>(u_value & 0b01111111);
                u_value >>= 7;

                if (u_value != 0)
                    temp |= 0b10000000;
                times++;
            } while (u_value != 0);
            return times;
        }
    };

    class uuid final {
    public:
        static uuid from_string(std::string_view string);

        static uuid invalid() { return uuid(0, 0); }

        uuid() = default;

        uuid(
                uint64_t most_sig,
                uint64_t least_sig) noexcept;

        [[nodiscard]] bool operator==(const uuid &other) noexcept;

        [[nodiscard]] uint64_t most_significant_bits() const noexcept;

        [[nodiscard]] uint64_t least_significant_bits() const noexcept;

        [[nodiscard]] std::string to_string() const;

        bool valid() const noexcept;

    private:
        uint64_t _most_sig_bits;
        uint64_t _least_sig_bits;
    };

    class chat {
    public:
        chat() = default;

        explicit chat(const std::string &json) {
//            chat_data = nlohmann::json::parse(json.begin(), json.end());
chat_data = json;
        }

        std::string chat_data;
//        nlohmann::json chat_data;
    };

    class slot {
    public:
        std::uint8_t item_count;
        std::int32_t item_id;
        nbt::node nbt_data;
    };

    class identifier {
    public:
        explicit identifier(const std::string &str) {
            auto split_idx = 0;
            for (auto i = 0; i < str.size(); i++)
                if (str[i] == ':') {
                    split_idx = i;
                    break;
                }

            name_space = str.substr(0, split_idx - 1);
            id = str.substr(split_idx + 1);
        }

        std::string name_space;
        std::string id;
    };

    class entity_metadata {
    public:
        std::unordered_map<std::byte,
        std::variant<
                std::byte,
                std::int16_t,
                std::int32_t,
                float,
                std::string,
                slot,
                glm::ivec3,
                glm::vec3
        >> data;
    };

}