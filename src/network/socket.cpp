#include "socket.h"

let::network::socket::socket(const std::string &host, std::uint16_t port)
        : _socket({host, port}) {
    ZoneScopedN("socket::construct");
//    if (!_socket)
//        fmt::print("Error connecting to [{}:{}]\n\tDescription: {}\n", host, port, _socket.last_error_str());
//    else {
//        fmt::print("Created socket connection \n\t{}\n", _socket.address().to_string());
        _socket.set_non_blocking(true);
//    }
}

void let::network::socket::send(let::network::byte_buffer &data) {
    ZoneScopedN("socket::send");
    const auto byte_count = data.size();
    const auto bytes = data.next_bytes(byte_count);
    data.clear();

    auto value = _socket.write(bytes.data(), bytes.size());
//    if (value < 0)
//        fmt::print("There was en error writing to socket: \n\t{}", _socket.last_error_str());
}

void let::network::socket::receive(let::network::byte_buffer &data, size_t byte_size) {
    ZoneScopedN("socket::receive");

    if (byte_size == 94)
        auto a = 5;

    auto buffer = std::array<std::byte, 1024>();

    auto total_read = size_t(0);

    while (total_read < byte_size) {
        const auto bytes_left = byte_size - total_read;
        const auto to_read = glm::min(bytes_left, buffer.size());

        const auto read = ::recv(_socket.handle(), reinterpret_cast<char*>(buffer.data()),
                                 int(to_read), 0);
        if (read > 0) {
            data.write_bytes(buffer.data(), read);
            total_read += read;
        }

        if (read < 0)
        {
            const auto err_code = WSAGetLastError();
            if (err_code == WSAEWOULDBLOCK)
                continue;
//            fmt::print("There was en error receiving to buffer. Error code: {}\n", err_code);
        }
    }
}

int let::network::socket::ask(let::network::byte_buffer &data, size_t data_size) {
    ZoneScopedN("socket::ask");
    // Todo: Use std::array & make it a while loop
    auto buffer = std::vector<std::byte>(data_size);

    const auto read = ::recv(_socket.handle(), reinterpret_cast<char*>(buffer.data()), data_size, MSG_PEEK);

    if (read > 0) {
        data.write_bytes(buffer.data(), read);
        return read;
    } else if (read == 0) {
        return 0;
    } else if (read < 0)
    {
        const auto err_code = WSAGetLastError();
        if (err_code == WSAEWOULDBLOCK)
            return 0;
        // Todo: need exception here
    }

    return 0;
}

std::byte let::network::socket::read_byte() {
    char result;

    while (true) {
        const auto read = ::recv(_socket.handle(), &result, 1, 0);

        if (read < 0)
            if (WSAGetLastError() == WSAEWOULDBLOCK)
                continue;;

        if (read != 1)
            continue;

        return std::byte(result);
    }
}

void let::network::socket::disconnect() {
    ZoneScopedN("socket::disconnect");
    _socket.close();
}

bool let::network::socket::connected() const noexcept {
    return _socket.is_connected();
}

bool let::network::socket::open() const noexcept {
    return _socket.is_open();
}
