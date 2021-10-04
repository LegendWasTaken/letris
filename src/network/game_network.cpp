//
// Created by howto on 2/9/2021.
//

#include "game_network.h"

let::network::game::game() : _compression_threshold(-1) {
    _processing_thread = std::thread([this] {
//        _process();
    });
}

let::network::game::~game() {
    _processing = false;
    _processing_thread.join();
}

void let::network::game::send_data(const std::vector<std::byte> &data) {

}

std::vector<std::byte> let::network::game::incoming() {
    auto out = _incoming.next_bytes(_incoming.size());
    _incoming.clear();
    return out;
}

let::network::game::connection_status let::network::game::status() const noexcept {
    return _status;
}

void let::network::game::connect(const std::string &address, const std::uint16_t port) {
    _server_socket = let::network::socket(address, port);
    _incoming.clear();
    _outgoing.clear();

    _status = connection_status::connecting;

    auto buffer = let::network::byte_buffer();
    let::packets::write<let::packets::state::handshaking>::handshake(buffer, packets::handshake_state::login, port,
                                                                     address);
    _server_socket.send(buffer);

    let::packets::write<let::packets::state::login>::login_start(buffer, "Test");
    _server_socket.send(buffer);
}

void let::network::game::disconnect() {
    _status = connection_status::disconnected;
    _server_socket.disconnect();
}

void let::network::game::_process() {
    auto incoming = let::network::byte_buffer();
    auto processed_incoming = let::network::byte_buffer();
//    while (_processing) {
    _server_socket.receive(incoming, 0);
    while (incoming.size() > 0) {

        if (_compression_threshold != -1) {
            auto packet_length = let::var_int();
            auto data_length = let::var_int();
            let::network::decoder::read(incoming, packet_length);
            let::network::decoder::read(incoming, data_length);

            if (data_length.val == 0) {
                // If data_length in the packet data is set to 0, the packet is not compressed.
                // We can assume the server will always abide by this, even if it doesn't. It wont matter, we handle it properly
                const auto decompressed_length = packet_length.val - 1; // minus 1 from the 0 of data length

                const auto packet_data = incoming.next_bytes(decompressed_length);

                let::network::encoder::write(processed_incoming, let::var_int(decompressed_length));
                processed_incoming.write_bytes(packet_data.data(), packet_data.size());
            } else {
                decompress_packet(incoming, data_length.val, packet_length.val, processed_incoming);
            }
        } else {
            // The packet structure is as follows
            // var int,
            // var int,
            // data

            auto size = let::var_int();
            let::network::decoder::read(incoming, size);

            const auto data = incoming.next_bytes(size.val);
            let::network::encoder::write(processed_incoming, size);
            processed_incoming.write_bytes(data.data(), data.size());
        }

        switch (_status) {
            case connection_status::connecting:
                _handle_connecting_packet(processed_incoming);
                break;
            case connection_status::connected:
                _handle_connected_packet(processed_incoming);
                break;
            case connection_status::disconnected:
                // Shouldn't ever reach here...
                break;
        }
    }

//    }
}

void let::network::game::_handle_connecting_packet(let::network::byte_buffer &buffer) {
    const auto header = let::packets::reader::peek_header(buffer);
    switch (header.id.val) {
        case 0x00: {
            const auto packet = let::packets::read<packets::state::login>::disconnect(buffer);

            const auto test = 5;
        }
            break;
        case 0x01: {
            const auto packet = let::packets::read<packets::state::login>::encryption_request(buffer);

            const auto a = 5;
        }
            break;
        case 0x02: {
            const auto packet = let::packets::read<packets::state::login>::login_success(buffer);
            // We dont use any of the fields for now
            _status = connection_status::connected;
        }
            break;
        case 0x03: {
            const auto packet = let::packets::read<packets::state::login>::set_compression(buffer);
            _compression_threshold = packet.threshold.val;
        }
            break;
    }
}

void let::network::game::_handle_connected_packet(let::network::byte_buffer &buffer) {
    const auto header = let::packets::reader::peek_header(buffer);
    switch (header.id.val)
    {
        case 0x0: {
            const auto packet = let::packets::read<packets::state::play>::keep_alive(buffer);
            break;
        }

        case 0x1: {
            const auto packet = let::packets::read<packets::state::play>::join_game(buffer);

            // Reply with the client brand, and client settings


            break;
        }

        case 0x2: {
            const auto packet = let::packets::read<packets::state::play>::chat_message(buffer);
            break;
        }

        case 0x3: {
            const auto packet = let::packets::read<packets::state::play>::time_update(buffer);
            break;
        }

        case 0x4: {
            const auto packet = let::packets::read<packets::state::play>::entity_equipment(buffer);
            break;
        }

        case 0x5: {
            const auto packet = let::packets::read<packets::state::play>::spawn_position(buffer);
            break;
        }

        case 0x6: {
            const auto packet = let::packets::read<packets::state::play>::update_health(buffer);
            break;
        }

        case 0x7: {
            const auto packet = let::packets::read<packets::state::play>::respawn(buffer);
            break;
        }

        case 0x8: {
            const auto packet = let::packets::read<packets::state::play>::player_pos_and_look(buffer);
            break;
        }

        case 0x9: {
            const auto packet = let::packets::read<packets::state::play>::held_item_change(buffer);
            break;
        }

        case 0xA: {
            const auto packet = let::packets::read<packets::state::play>::use_bed(buffer);
            break;
        }

        case 0xB: {
            const auto packet = let::packets::read<packets::state::play>::animation(buffer);
            break;
        }

        case 0xC: {
            const auto packet = let::packets::read<packets::state::play>::spawn_player(buffer);
            break;
        }

        case 0xD: {
            const auto packet = let::packets::read<packets::state::play>::collect_item(buffer);
            break;
        }

        case 0xE: {
            const auto packet = let::packets::read<packets::state::play>::spawn_object(buffer);
            break;
        }

        case 0xF: {
            const auto packet = let::packets::read<packets::state::play>::spawn_mob(buffer);
            break;
        }

        case 0x10: {
            const auto packet = let::packets::read<packets::state::play>::spawn_painting(buffer);
            break;
        }

        case 0x11: {
            const auto packet = let::packets::read<packets::state::play>::spawn_experience_orb(buffer);
            break;
        }

        case 0x12: {
            const auto packet = let::packets::read<packets::state::play>::entity_velocity(buffer);
            break;
        }

        case 0x13: {
            const auto packet = let::packets::read<packets::state::play>::destroy_entities(buffer);
            break;
        }

        case 0x14: {
            const auto packet = let::packets::read<packets::state::play>::entity(buffer);
            break;
        }

        case 0x15: {
            const auto packet = let::packets::read<packets::state::play>::entity_relative_move(buffer);
            break;
        }

        case 0x16: {
            const auto packet = let::packets::read<packets::state::play>::entity_look(buffer);
            break;
        }

        case 0x17: {
            const auto packet = let::packets::read<packets::state::play>::entity_look_and_relative_move(buffer);
            break;
        }

        case 0x18: {
            const auto packet = let::packets::read<packets::state::play>::entity_teleport(buffer);
            break;
        }

        case 0x19: {
            const auto packet = let::packets::read<packets::state::play>::entity_head_look(buffer);
            break;
        }

        case 0x1A: {
            const auto packet = let::packets::read<packets::state::play>::entity_status(buffer);
            break;
        }

        case 0x1B: {
            const auto packet = let::packets::read<packets::state::play>::attach_entity(buffer);
            break;
        }

        case 0x1C: {
            const auto packet = let::packets::read<packets::state::play>::entity_data(buffer);
            break;
        }

        case 0x1D: {
            const auto packet = let::packets::read<packets::state::play>::entity_effect(buffer);
            break;
        }

        case 0x1E: {
            const auto packet = let::packets::read<packets::state::play>::remove_entity_effect(buffer);
            break;
        }

        case 0x1F: {
            const auto packet = let::packets::read<packets::state::play>::set_experience(buffer);
            break;
        }

        case 0x20: {
            const auto packet = let::packets::read<packets::state::play>::entity_properties(buffer);
            break;
        }

        case 0x21: {
            const auto packet = let::packets::read<packets::state::play>::chunk_data(buffer);
            break;
        }

        case 0x22: {
            const auto packet = let::packets::read<packets::state::play>::multi_block_change(buffer);
            break;
        }

        case 0x23: {
            const auto packet = let::packets::read<packets::state::play>::block_change(buffer);
            break;
        }

        case 0x24: {
            const auto packet = let::packets::read<packets::state::play>::block_action(buffer);
            break;
        }

        case 0x25: {
            const auto packet = let::packets::read<packets::state::play>::block_break_animation(buffer);
            break;
        }

        case 0x26: {
            const auto packet = let::packets::read<packets::state::play>::map_chunk_bulk(buffer);
            break;
        }

        case 0x27: {
            const auto packet = let::packets::read<packets::state::play>::explosion(buffer);
            break;
        }

        case 0x28: {
            const auto packet = let::packets::read<packets::state::play>::effect(buffer);
            break;
        }

        case 0x29: {
            const auto packet = let::packets::read<packets::state::play>::sound_effect(buffer);
            break;
        }

        case 0x2A: {
            const auto packet = let::packets::read<packets::state::play>::particle(buffer);
            break;
        }

        case 0x2B: {
            const auto packet = let::packets::read<packets::state::play>::change_game_state(buffer);
            break;
        }

        case 0x2C: {
            const auto packet = let::packets::read<packets::state::play>::spawn_global_entity(buffer);
            break;
        }

        case 0x2D: {
            const auto packet = let::packets::read<packets::state::play>::open_window(buffer);
            break;
        }

        case 0x2E: {
            const auto packet = let::packets::read<packets::state::play>::close_window(buffer);
            break;
        }

        case 0x2F: {
            const auto packet = let::packets::read<packets::state::play>::set_slot(buffer);
            break;
        }

        case 0x30: {
            const auto packet = let::packets::read<packets::state::play>::window_items(buffer);
            break;
        }

        case 0x31: {
            const auto packet = let::packets::read<packets::state::play>::window_property(buffer);
            break;
        }

        case 0x32: {
            const auto packet = let::packets::read<packets::state::play>::confirm_transaction(buffer);
            break;
        }

        case 0x33: {
            const auto packet = let::packets::read<packets::state::play>::update_sign(buffer);
            break;
        }

        case 0x34: {
            const auto packet = let::packets::read<packets::state::play>::map(buffer);
            break;
        }

        case 0x35: {
            const auto packet = let::packets::read<packets::state::play>::update_block_entity(buffer);
            break;
        }

        case 0x36: {
            const auto packet = let::packets::read<packets::state::play>::open_sign_editor(buffer);
            break;
        }

        case 0x37: {
            const auto packet = let::packets::read<packets::state::play>::statistics(buffer);
            break;
        }

        case 0x38: {
            const auto packet = let::packets::read<packets::state::play>::player_list_item(buffer);
            break;
        }

        case 0x39: {
            const auto packet = let::packets::read<packets::state::play>::player_abilities(buffer);
            break;
        }

        case 0x3A: {
            const auto packet = let::packets::read<packets::state::play>::tab_complete(buffer);
            break;
        }

        case 0x3B: {
            const auto packet = let::packets::read<packets::state::play>::scoreboard_objective(buffer);
            break;
        }

        case 0x3C: {
            const auto packet = let::packets::read<packets::state::play>::update_score(buffer);
            break;
        }

        case 0x3D: {
            const auto packet = let::packets::read<packets::state::play>::display_scoreboard(buffer);
            break;
        }

        case 0x3E: {
            const auto packet = let::packets::read<packets::state::play>::teams(buffer);
            break;
        }

        case 0x3F: {
            const auto packet = let::packets::read<packets::state::play>::plugin_message(buffer);
            break;
        }

        case 0x40: {
            const auto packet = let::packets::read<packets::state::play>::disconnect(buffer);
            break;
        }

        case 0x41: {
            const auto packet = let::packets::read<packets::state::play>::server_difficulty(buffer);
            break;
        }

        case 0x42: {
            const auto packet = let::packets::read<packets::state::play>::combat_event(buffer);
            break;
        }

        case 0x43: {
            const auto packet = let::packets::read<packets::state::play>::camera(buffer);
            break;
        }

        case 0x44: {
            const auto packet = let::packets::read<packets::state::play>::world_border(buffer);
            break;
        }

        case 0x45: {
            const auto packet = let::packets::read<packets::state::play>::title(buffer);
            break;
        }

        case 0x46: {
            const auto packet = let::packets::read<packets::state::play>::set_compression(buffer);
            break;
        }

        case 0x47: {
            const auto packet = let::packets::read<packets::state::play>::player_list_header_and_footer(buffer);
            break;
        }

        case 0x48: {
            const auto packet = let::packets::read<packets::state::play>::resource_pack_send(buffer);
            break;
        }
    }
}

void let::network::game::decompress_packet(let::network::byte_buffer &source, size_t decompressed_size,
                                           size_t compressed_size, let::network::byte_buffer &target) {

    auto decompressed_data = std::vector<std::byte>(decompressed_size);

    let::network::encoder::write(target, let::var_int(decompressed_size));

    const auto result = zng_uncompress(reinterpret_cast<uint8_t *>(decompressed_data.data()), &decompressed_size,
                   reinterpret_cast<uint8_t *>(source.data()), compressed_size);

    if (result != Z_OK)
        fmt::print("Decompression Failed: {}", zng_zError(result));

}

void let::network::game::compress_packet(let::network::byte_buffer &source, let::network::byte_buffer &target) {

}
