#pragma once

#include <string>
#include <numeric>

#include <network/byte_buffer.h>

#include <network/encoder.h>
#include <network/decoder.h>

namespace let::packets
{
  enum class state
  {
    play, status, login, handshaking,
  };

  enum class target
  {
    server, client
  };

  class writer
  {
  protected:
    static void write_header(std::int32_t length, std::int32_t packet_id, let::network::byte_buffer &buffer);
  };

  template <state State>
  class write : public writer
  {

  };

  template<>
  class write<state::play> : public writer
  {

  };

  template<>
  class write<state::status> : public writer
  {
  public:
    static void request(let::network::byte_buffer& buffer);
  };

  template<>
  class write<state::login> : public writer
  {

  };

  template<>
  class write<state::handshaking> : public writer
  {

  };

}
