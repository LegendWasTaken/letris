#pragma once

#include <string>
#include <array>

#include <sockpp/tcp_connector.h>
#include <sockpp/socket.h>

#include <fmt/core.h>

#include <network/packets.h>
#include <network/byte_buffer.h>

#include <tracy/Tracy.hpp>

namespace let::network {
    class socket {
    public:
        socket() = default;

        socket(const std::string &host, std::uint16_t port);

        void send(let::network::byte_buffer &data);

        void receive(let::network::byte_buffer &data, size_t byte_size);

        void disconnect();

        [[nodiscard]] bool connected() const noexcept;

        [[nodiscard]] bool open() const noexcept;

    private:
        sockpp::tcp_connector _socket;
    };
}
