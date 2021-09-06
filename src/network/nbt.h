#pragma once

#include <cstdint>
#include <string_view>
#include <optional>
#include <variant>

#include <network/byte_buffer.h>

namespace let::nbt
{
    enum class tag_type : std::uint8_t
    {
        END,
        BYTE,
        SHORT,
        INT,
        LONG,
        FLOAT,
        DOUBLE,
        BYTE_ARRAY,
        STRING,
        LIST,
        COMPOUND,
        INT_ARRAY,
        LONG_ARRAY
    };

    class node
    {
    public:
        [[nodiscard]] const node *get_node(std::string_view value) const;

        [[nodiscard]] static node read(let::network::byte_buffer &buffer);

    private:
        static node _read_node(
                let::network::byte_buffer &buffer,
                tag_type          parent    = tag_type::END,
                tag_type          list_type = tag_type::END);

        static void _read_value(let::network::byte_buffer &buffer, node &node);

        tag_type _type;
        std::variant<
                std::byte,
                std::int16_t,
                std::int32_t,
                std::int64_t,
                float,
                double,
                std::vector<std::int8_t>,
                std::string,
                std::vector<node>,
                std::vector<std::int32_t>,
                std::vector<std::int64_t>>
                _value;

        std::optional<std::string> _name;
    public:

        template <typename T>
        [[nodiscard]] std::optional<std::reference_wrapper<T>> get()
        {
            if (T* ptr = std::get_if<T>(_value); ptr) {
                return *ptr;
            }
            return {};
        }

    };

}    // namespace snp::nbt
