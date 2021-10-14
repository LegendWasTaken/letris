#include "world.h"

void let::world::process_packets(let::network::byte_buffer &buffer, let::network::byte_buffer &outgoing) {
    outgoing.clear();

    while (!buffer.empty()) {
        const auto header = let::packets::reader::peek_header(buffer);
        switch (header.id.val) {
            case 0x01: {
                const auto client_brand = std::string("Letris 1.8.9");
                auto brand_bytes = std::vector<std::byte>();
                for (auto letter : client_brand)
                    brand_bytes.push_back(std::byte(letter));

                let::packets::write<packets::state::play>::client_settings(outgoing, "en_GB", std::byte(8), std::byte(0), false, 0);
                let::packets::write<packets::state::play>::plugin_message(outgoing, "minecraft:brand", brand_bytes);
                break;
            }
            default:
                LET_EXCEPTION(exception::source_type::world, "Attempted to process a non implemented packet: ID: {}, Length: {}",
                              header.id.val, header.length.val);
        }
    }
}
