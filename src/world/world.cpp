#include "world.h"

void let::world::process_packets(let::network::byte_buffer &buffer) {
    while (!buffer.empty()) {
        const auto header = let::packets::reader::peek_header(buffer);
        switch (header.id.val) {
            case 0x01:



                break;

            default:
                LET_EXCEPTION(exception::source_type::world, "Attempted to process a non implemented packet: ID: {}, Length: {}",
                              header.id.val, header.length.val);
        }
    }
}
