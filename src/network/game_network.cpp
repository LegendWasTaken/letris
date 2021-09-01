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
    auto to_process = let::network::byte_buffer();
//    while (_processing) {
        _server_socket.receive(to_process);
        if (to_process.size() > 0) {
            switch (_status) {
                case connection_status::connecting:
                    _handle_connecting_packets(to_process);
                    break;
                case connection_status::connected:
                    _handle_connected_packets(to_process);
                    break;
                case connection_status::disconnected:
                    // Shouldn't ever reach here...
                    break;
            }
        }

//    }
}

void let::network::game::_handle_connecting_packets(let::network::byte_buffer &buffer) {
    while (buffer.size() > 0) {
        auto header = let::packets::reader::peek_header(buffer);
        switch (header.id.val) {
            case 0x00: {

            }
                break;
            case 0x01: {

            }
                break;
            case 0x02: {
                const auto packet = let::packets::read<packets::state::login>::login_success(buffer);
                const auto a = 5;
            }
                break;
            case 0x03: {
                const auto packet = let::packets::read<packets::state::login>::set_compression(buffer);
                const auto a = 5;
            }
                break;
        }
    }
}

void let::network::game::_handle_connected_packets(let::network::byte_buffer &buffer) {

}
