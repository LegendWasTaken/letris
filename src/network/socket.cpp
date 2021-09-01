#include "socket.h"

let::network::socket::socket(const std::string &host, std::uint16_t port)
        : _socket({host, port}) {
    if (!_socket)
        fmt::print("Error connecting to [{}:{}]\n\tDescription: {}\n", host, port, _socket.last_error_str());
    else {
        fmt::print("Created socket connection \n\t{}\n", _socket.address().to_string());
        _socket.set_non_blocking(true);
    }
}

bool let::network::socket::send(let::network::byte_buffer &data) {
    const auto byte_count = data.size();
    const auto bytes = data._next_bytes(byte_count);
    data.clear();

    auto value = _socket.write(bytes.data(), bytes.size());
    if (value < 0) {
        fmt::print("There was en error writing to socket: \n\t{}", _socket.last_error_str());
        return false;
    }

    return true;
}

bool let::network::socket::receive(let::network::byte_buffer &data) {
    auto any_data = false;

    auto buffer = std::array<std::byte, 1024>();
    while (true) {
        const auto read = ::recv(_socket.handle(), reinterpret_cast<char*>(buffer.data()),
                                 int(buffer.size()), 0);
        if (read > 0) {
            any_data = true;
            data.write_bytes(buffer.data(), read);
            continue;
        }

        if (read == 0)
            break;

        if (read < 0)
        {
            const auto err_code = WSAGetLastError();
            if (err_code == WSAEWOULDBLOCK)
                break;
            fmt::print("There was en error receiving to buffer. Error code: {}\n", err_code);
        }
    }
    return any_data;
}
