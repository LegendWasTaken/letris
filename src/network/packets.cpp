#include "packets.h"

void let::packets::writer::write_header(
        std::int32_t length,
        std::int32_t packet_id,
        let::network::byte_buffer &buffer)
{
  let::network::encoder::write(buffer, length);
  let::network::encoder::write(buffer, packet_id);
}

void let::packets::write<let::packets::state::status>::request(let::network::byte_buffer& buffer)
{
  write_header(1, 0x00, buffer);
}
