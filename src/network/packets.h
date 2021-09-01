#pragma once

#include <string>
#include <numeric>

#include <network/byte_buffer.h>

#include <network/encoder.h>
#include <network/decoder.h>

#include <nlohmann/json.hpp>

namespace let::packets {
    enum class state {
        play, status, login, handshaking,
    };

    enum class handshake_state {
        status = 0x01,
        login = 0x02
    };

    enum class target {
        server, client
    };

    class reader {
    protected:
        struct packet_header {
            let::var_int length;
            let::var_int id;
        };

        [[nodiscard]] static packet_header read_header(let::network::byte_buffer &buffer);

    public:
        struct incoming_packet {
            packet_header header;
        };
    };

    template<state State>
    class read : public reader {

    };

    class writer {
    protected:
        static void write_header(let::network::byte_buffer &buffer, std::int32_t length, std::int32_t packet_id);
    };

    template<state State>
    class write : public writer {

    };

    template<>
    class write<state::play> : public writer {

    };

    template<>
    class read<state::play> : public reader {

    };

    template<>
    class write<state::status> : public writer {
    public:
        static void request(let::network::byte_buffer &buffer);

        static void ping(let::network::byte_buffer &buffer, std::int64_t payload);
    };

    template<>
    class read<state::status> : public reader {
    public:
        struct response_packet : public reader::incoming_packet {
            struct {
                std::string name;
            } version;

            struct {
                std::uint32_t players_max;
                std::uint32_t online;
            } players;

            struct {
                std::string text;
            } description;
        };

        [[nodiscard]] static response_packet response(let::network::byte_buffer &buffer);

        struct pong_packet : public read::incoming_packet {
            std::int64_t payload;
        };

        [[nodiscard]] static pong_packet pong(let::network::byte_buffer &buffer);
    };


    template<>
    class write<state::login> : public writer {

    };

    template<>
    class write<state::handshaking> : public writer {
    public:
        struct handshake_data {
            enum class state {
                status = 0x01,
                login = 0x02
            };

            handshake_data::state next_state;
            std::uint16_t port;
            std::string host;
        };

        static void handshake(let::network::byte_buffer &buffer, const handshake_state state, const std::uint16_t port,
                              const std::string &address);

    };
}
