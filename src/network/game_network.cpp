#include "game_network.h"

#include <spdlog/spdlog.h>

let::network::game::game() : _compression_threshold(-1) {
    _processing_thread = std::thread([this] {
//        _process();
    });
}

let::network::game::~game() {
    _processing = false;
    _processing_thread.join();
}

void let::network::game::send_data(let::network::byte_buffer &data) {
    auto outgoing = data.next_bytes(data.size());
    data.clear();
    _outgoing.write_bytes(outgoing.data(), outgoing.size());
    outgoing.clear();
}

let::network::byte_buffer let::network::game::incoming() {
    auto out = _incoming;
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
    static auto incoming = let::network::byte_buffer();
    static auto outgoing = let::network::byte_buffer();

    while (_read_packet(incoming)) {
        switch (_status) {
            case connection_status::connecting:
                _handle_connecting_packet(incoming);
                break;
            case connection_status::connected: {
                const auto packet = incoming.next_bytes(incoming.size());
                _incoming.write_bytes(packet.data(), packet.size());
                incoming.clear();
                break;
            }
            case connection_status::disconnected:
                // Shouldn't ever reach here...
                break;
            default:
                LET_EXCEPTION(exception::source_type::network, "In an unreachable state");
        }
    }

    while (_outgoing.has_left()) {
        // First step is to compress before sending
        _compress_packet(_outgoing, outgoing);

        // Todo: Encryption
    }
    // Now YEET the packets onto the internet
    _server_socket.send(outgoing);

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
    buffer.clear();
}

void let::network::game::_decompress_packet(let::network::byte_buffer &source, size_t decompressed_size,
                                           size_t compressed_size, let::network::byte_buffer &target) {

    auto decompressed_data = std::vector<std::byte>(decompressed_size);

    const auto result = zng_uncompress(reinterpret_cast<uint8_t *>(decompressed_data.data()), &decompressed_size,
                   reinterpret_cast<uint8_t *>(source.data()), compressed_size);

    if (result != Z_OK)
        LET_EXCEPTION(exception::source_type::network, "Failed to decompressed buffer");

    target.write_bytes(decompressed_data.data(), decompressed_data.size());
}

void let::network::game::_compress_packet(let::network::byte_buffer &source, let::network::byte_buffer &target) {
    auto packet_length = let::var_int();
    auto packet_id = let::var_int();

    let::network::decoder::read(source, packet_length);
    let::network::decoder::read(source, packet_id);

    const auto packet_data = source.next_bytes(packet_length.val - packet_id.length());

    if (_compression_threshold != -1) {
        // Use the compressed packet format
        auto compressed_length = let::var_int();
        auto data_length = let::var_int();

        // Create the data_to_send, by stepping back. And then copying
        source.step_back(packet_data.size() + packet_id.length());
        auto data_to_send = source.next_bytes(packet_data.size() + packet_id.length());

        if (packet_length.val > _compression_threshold) {
            auto compressed = std::vector<std::byte>(zng_compressBound(data_to_send.size()));
            auto size = compressed.size();

            const auto result = zng_compress2(reinterpret_cast<uint8_t *>(compressed.data()), &size,
                                        reinterpret_cast<const uint8_t *>(data_to_send.data()), data_to_send.size(), Z_BEST_COMPRESSION);
            if (result != Z_OK)
                LET_EXCEPTION(exception::source_type::network, "Failed to compress buffer");

            std::swap(data_to_send, compressed);
        } else {
            data_length.val = 0;
            compressed_length.val = data_length.length() + data_to_send.size();
        }

        let::network::encoder::write(target, compressed_length);
        let::network::encoder::write(target, data_length);
        target.write_bytes(data_to_send.data(), data_to_send.size());
    } else {
        // Dont compress the packet, regular format, so just copy paste the packet

        // Todo: Optimize the whole "copy buffer into another buffer"
        let::network::encoder::write(target, packet_length);
        let::network::encoder::write(target, packet_id);
        target.write_bytes(packet_data.data(), packet_data.size());
    }
}

bool let::network::game::_read_packet(let::network::byte_buffer &data) {
    static auto incoming = let::network::byte_buffer();
    static auto intermediate_buffer = let::network::byte_buffer();
    incoming.clear();
    intermediate_buffer.clear();

    // Very expensive way of asking if there's any bytes in the stream
    const auto read = _server_socket.ask(incoming, 1);
    incoming.clear();
    if (read == 0)
        return false;

    if (_compression_threshold != -1) {
        auto packet_length = _read_var_int();
        auto data_length = _read_var_int();

        if (data_length.val == 125543)
            const auto a = 5;

        const auto packet_data_size = packet_length.val - data_length.length();

        if (packet_data_size > 0)
            _server_socket.receive(incoming, packet_data_size);

        if (data_length.val != 0) {
            _decompress_packet(incoming, data_length.val, packet_data_size, intermediate_buffer);
            let::network::encoder::write(data, let::var_int(data_length.val));
            auto packet_data = intermediate_buffer.next_bytes(intermediate_buffer.size());
            data.write_bytes(packet_data.data(), packet_data.size());
        } else {
            let::network::encoder::write(data, let::var_int(packet_data_size));
            auto packet_data = incoming.next_bytes(packet_data_size);
            data.write_bytes(packet_data.data(), packet_data.size());
        }
    } else {
        auto data_length = _read_var_int();

        _server_socket.receive(incoming, data_length.val);

        let::network::encoder::write(data, data_length);
        auto packet_data = incoming.next_bytes(data_length.val);
        data.write_bytes(packet_data.data(), packet_data.size());
    }

    return true;
}

let::var_int let::network::game::_read_var_int() {
    int numRead = 0;
    int32_t result  = 0;
    uint8_t read;
    do {
        read       = static_cast<uint8_t>(_server_socket.read_byte());
        int val = (read & 0b01111111);
        result |= (val << (7 * numRead));

        numRead++;
        if (numRead > 5) throw std::runtime_error("VarInt too big");
    } while ((read & 0b10000000) != 0);

    auto res = let::var_int();
    return res.val = result, res;
}
