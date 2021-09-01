#include "packets.h"

let::packets::reader::packet_header let::packets::reader::peek_header(let::network::byte_buffer &buffer) {
    auto header = packet_header();
    const auto count = buffer.size();
    let::network::decoder::read(buffer, header.length);
    let::network::decoder::read(buffer, header.id);
    buffer.step_back(count - buffer.size());
    return header;
}

let::packets::reader::packet_header let::packets::reader::read_header(let::network::byte_buffer &buffer) {
    auto header = packet_header();
    let::network::decoder::read(buffer, header.length);
    let::network::decoder::read(buffer, header.id);
    return header;
}

void let::packets::writer::write_header(
        let::network::byte_buffer &buffer,
        std::int32_t length,
        std::int32_t packet_id) {
    let::network::encoder::write(buffer, let::var_int(length));
    let::network::encoder::write(buffer, let::var_int(packet_id));
}

void let::packets::write<let::packets::state::status>::request(let::network::byte_buffer &buffer) {
    write_header(buffer, 1, 0x00);
}

void let::packets::write<let::packets::state::status>::ping(let::network::byte_buffer &buffer, std::int64_t payload) {
    write_header(buffer, 9, 0x01);
    let::network::encoder::write(buffer, payload);
}

let::packets::read<let::packets::state::status>::response_packet
let::packets::read<let::packets::state::status>::response(let::network::byte_buffer &buffer) {
    auto response = response_packet();
    response.header = read_header(buffer);

    auto json_str = std::string();
    let::network::decoder::read(buffer, json_str);

    const auto json = nlohmann::json::parse(json_str.begin(), json_str.end());

    json.at("version").at("name").get_to(response.version.name);
    json.at("players").at("max").get_to(response.players.players_max);
    json.at("players").at("online").get_to(response.players.online);
    json.at("description").get_to(response.description.text);

    return response;
}

let::packets::read<let::packets::state::status>::pong_packet
let::packets::read<let::packets::state::status>::pong(let::network::byte_buffer &buffer) {
    auto pong = pong_packet();
    pong.header = read_header(buffer);

    let::network::decoder::read(buffer, pong.payload);
    return pong;
}

void let::packets::write<let::packets::state::login>::login_start(let::network::byte_buffer &buffer,
                                                                  const std::string &name) {

    const auto packet_length = let::var_int(name.size()).length() + name.size() + 1;

    write_header(buffer, packet_length, 0x00);
    let::network::encoder::write(buffer, name);
}

let::packets::read<let::packets::state::login>::login_success_packet
let::packets::read<let::packets::state::login>::login_success(let::network::byte_buffer &buffer) {
    auto login_success = login_success_packet();
    login_success.header = read_header(buffer);

    let::network::decoder::read(buffer, login_success.uuid);
    let::network::decoder::read(buffer, login_success.username);
    return login_success;
}

let::packets::read<let::packets::state::login>::set_compression_packet
let::packets::read<let::packets::state::login>::set_compression(let::network::byte_buffer &buffer) {
    auto set_compression = set_compression_packet();

    set_compression.header = read_header(buffer);
    let::network::decoder::read(buffer, set_compression.threshold);

    return set_compression;
}

void let::packets::write<let::packets::state::handshaking>::handshake(let::network::byte_buffer &buffer,
                                                                      const handshake_state state,
                                                                      const std::uint16_t port,
                                                                      const std::string &address) {
    const auto protocol_version = let::var_int(47);
    const auto next_state = let::var_int(static_cast<std::int32_t>(state));

    const auto packet_length = protocol_version.length() +
                               let::var_int(address.size()).length() +
                               address.size() +
                               sizeof(std::uint16_t) +
                               next_state.length() + 1;

    write_header(buffer, packet_length, 0x00);
    let::network::encoder::write(buffer, protocol_version);
    let::network::encoder::write(buffer, address);
    let::network::encoder::write(buffer, port);
    let::network::encoder::write(buffer, next_state);
}
