#include "nbt.h"

#include <common/network/decoder.h>

namespace {
    [[nodiscard]] let::nbt::tag_type read_type(let::network::byte_buffer &buffer) {
        return static_cast<let::nbt::tag_type>(buffer.next_byte());
    }

    [[nodiscard]] std::string read_string(let::network::byte_buffer &buffer) {
        auto string_size = std::uint16_t();
        let::network::decoder::read(buffer, string_size);
        const auto string_data = buffer.next_bytes(string_size);

        auto string = std::string(string_size, '\0');
        for (auto i = 0; i < string_size; i++) string[i] = static_cast<char>(string_data[i]);

        return string;
    }
}    // namespace

let::nbt::node let::nbt::node::read(let::network::byte_buffer &buffer) {
    return _read_node(buffer);
}

std::optional<let::nbt::node> let::nbt::node::read_optional(let::network::byte_buffer &buffer) {
    const auto type = ::read_type(buffer);
    if (type == tag_type::END)
        return std::nullopt;

    buffer.step_back(1);
    return _read_node(buffer);
}

let::nbt::node
let::nbt::node::_read_node(let::network::byte_buffer &buffer, tag_type parent, tag_type list_type) {
    // We use end to signify that we're on the first node, it should only get called with `end`
    // in the first call, and never under (only COMPOUND, or LIST)
    auto value = node();
    if (parent == tag_type::END) {
        value._type = ::read_type(buffer);
        value._name = ::read_string(buffer);
    } else if (parent == tag_type::LIST) {
        value._type = list_type;
    } else if (parent == tag_type::COMPOUND) {
        value._type = ::read_type(buffer);
        if (value._type == tag_type::END) return value;

        value._name = ::read_string(buffer);
    }
    _read_value(buffer, value);

    return value;
}

void let::nbt::node::_read_value(let::network::byte_buffer &buffer, let::nbt::node &node) {
    switch (node._type) {
        case tag_type::END:
            break;
        case tag_type::BYTE: {
            auto val = std::byte();
            let::network::decoder::read(buffer, val);
            node._value = val;
        };
            break;
        case tag_type::SHORT: {
            auto val = std::int16_t();
            let::network::decoder::read(buffer, val);
            node._value = val;
        };
            break;
        case tag_type::INT: {
            auto val = std::int32_t();
            let::network::decoder::read(buffer, val);
            node._value = val;
        };
            break;
        case tag_type::LONG: {
            auto val = std::int64_t();
            let::network::decoder::read(buffer, val);
            node._value = val;
        };
            break;
        case tag_type::FLOAT: {
            auto val = float();
            let::network::decoder::read(buffer, val);
            node._value = val;
        };
            break;
        case tag_type::DOUBLE: {
            auto val = double();
            let::network::decoder::read(buffer, val);
            node._value = val;
        };
            break;
        case tag_type::BYTE_ARRAY: {
            auto length = std::int32_t();
            let::network::decoder::read(buffer, length);
            auto bytes = buffer.next_bytes(sizeof(std::byte) * length);

            auto vector = std::vector<std::int8_t>(length);

            node._value = vector;
            break;
        }
        case tag_type::STRING:
            node._value = ::read_string(buffer);
            break;
        case tag_type::LIST: {
            const auto child_type = static_cast<let::nbt::tag_type>(buffer.next_byte());

            auto children_count = std::int32_t();
            let::network::decoder::read(buffer, children_count);

            auto children = std::vector<nbt::node>(children_count);

            for (auto i = 0; i < children_count; i++)
                children[i] = _read_node(buffer, tag_type::LIST, child_type);

            node._value = std::move(children);
            break;
        }
        case tag_type::COMPOUND: {
            auto children = std::vector<nbt::node>();
            auto current_node = nbt::node();

            while ((current_node = _read_node(buffer, tag_type::COMPOUND))._type != tag_type::END)
                children.push_back(current_node);

            node._value = std::move(children);
            break;
        }
        case tag_type::INT_ARRAY: {
            auto length = std::int32_t();
            let::network::decoder::read(buffer, length);
            auto bytes = buffer.next_bytes(sizeof(std::int32_t) * length);

            auto vector = std::vector<std::int32_t>(length);

            node._value = vector;
            break;
        }
        case tag_type::LONG_ARRAY: {
            auto length = std::int32_t();
            let::network::decoder::read(buffer, length);
            auto bytes = buffer.next_bytes(sizeof(std::int64_t) * length);

            auto vector = std::vector<std::int64_t>(length);

            node._value = vector;
            break;
        }
    }
}

const let::nbt::node *let::nbt::node::get_node(std::string_view value) const {
    // I do not like this function one bit... Too much C++ bullshit going on
    const node *node_ptr = nullptr;

    if (_type != tag_type::COMPOUND) return node_ptr;    // Can't search by name

    const auto &nodes = std::get<std::vector<node>>(_value);

    for (auto i = 0; i < nodes.size(); i++)
        if (std::string_view(nodes[i]._name.value()) == value) node_ptr = &nodes[i];

    return node_ptr;
}
