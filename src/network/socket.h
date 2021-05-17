#pragma once

#include <string>
#include <array>

#include <sockpp/tcp_connector.h>
#include <sockpp/socket.h>

#include <fmt/core.h>

#include <network/packets.h>
#include <network/byte_buffer.h>

namespace let::network
{
  class socket
  {
  public:
    socket(const std::string &host, std::uint16_t port);

    [[nodiscard]] bool send(let::network::byte_buffer &data);

    [[nodiscard]] bool receive(let::network::byte_buffer &data);
  private:
    let::packets::state _state;

    sockpp::tcp_connector _socket;
  };
}
