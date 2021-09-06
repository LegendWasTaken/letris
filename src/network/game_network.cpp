//
// Created by howto on 2/9/2021.
//

#include "game_network.h"

let::network::game::game() {
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
    auto decompressed = let::network::byte_buffer();
//    while (_processing) {
    _server_socket.receive(incoming);
    while (incoming.size() > 0) {

        if (_compression_threshold != -1)
        {
            auto packet_length = let::var_int();
            auto data_length = let::var_int();
            let::network::decoder::read(incoming, packet_length);
            let::network::decoder::read(incoming, data_length);
            decompress_packet(incoming, decompressed);
        }

        switch (_status) {
            case connection_status::connecting:
                _handle_connecting_packet(decompressed);
                break;
            case connection_status::connected:
                _handle_connected_packet(decompressed);
                break;
            case connection_status::disconnected:
                // Shouldn't ever reach here...
                break;
        }
    }

//    }
}

void let::network::game::_handle_connecting_packet(let::network::byte_buffer &buffer) {
    auto header = let::packets::reader::peek_header(buffer);
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

}

void let::network::game::decompress_packet(let::network::byte_buffer &source, let::network::byte_buffer &target) {

}
