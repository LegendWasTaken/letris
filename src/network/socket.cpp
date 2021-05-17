#include "socket.h"

let::network::socket::socket(const std::string &host, std::uint16_t port)
: _socket({host, port})
{
  if (!_socket)
  {
    fmt::print("Error connecting to [{}:{}]\n\tDescription: {}\n", host, port, _socket.last_error_str());
  }
  else
  {
    fmt::print("Created socket connection \n\t{}\n", _socket.address().to_string());
  }
}

bool let::network::socket::send(let::network::byte_buffer &data)
{
  const auto byte_count = data.size();
  const auto bytes = data._next_bytes(byte_count);
  data.clear();

  auto value = _socket.write(bytes.data(), data.size());
  if (value < 0)
  {
    fmt::print("There was en error writing to socket: \n\t{}", _socket.last_error_str());
    return false;
  }

  return true;
}

bool let::network::socket::receive(let::network::byte_buffer &data)
{
  std::array<char, 16> reee {};
  _socket.read(reee.data(), 16);
  for (const auto byte : reee)
    data.write_byte(std::byte(byte));

  return false;
}
