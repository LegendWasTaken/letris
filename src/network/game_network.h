#pragma once

#include <network/socket.h>
#include <common/network/packets.h>

#include <common/exception.h>

#include <zlib-ng.h>

#include <vector>
#include <thread>
#include <mutex>

namespace let::network {
    class game {
    public:
        game();

        ~game();

        game(const game &other) = delete;

        game(game &&other) = delete;

        game &operator=(const game &other) = delete;

        game &operator=(game &&other) = delete;

        /// \brief Send data to the server currently connected to, only valid if mode is [game]
        /// \param data The data to send to the server
        void send_data(const std::vector<std::byte> &data);

        /// \brief Get the incoming game packet data, only valid if mode is [game]
        /// \return Incoming packet data as a byte vector
        [[nodiscard]] std::vector<std::byte> incoming();

        enum class connection_status {
            connecting,
            connected,
            disconnected
        };

        /// \brief Only valid if mode is [game]
        /// \return The current status of the server connection
        [[nodiscard]] connection_status status() const noexcept;

        /// \brief Connect to a minecraft server, this will set the mode to [game] and dispose other connections
        /// \param address Target server address, "127.0.0.1", "localhost"
        /// \param port The port to connect to the server, default is 25565
        void connect(const std::string &address, std::uint16_t port = 25565);


        /// \brief Disconnect from the currently connected server, only valid if the mode is [game]
        void disconnect();
        void _process();
    private:

        void _handle_connecting_packet(let::network::byte_buffer &buffer);

        void _decompress_packet(let::network::byte_buffer &source, size_t decompressed_size, size_t compressed_size, let::network::byte_buffer &target);

        void _compress_packet(let::network::byte_buffer &source, let::network::byte_buffer &target);

        /// \brief Attempt to read a single packet into the buffer
        /// \param data The buffer to read the decompressed and decrypted packet data into
        /// \return If there was a packet read into the buffer or not
        [[nodiscard]] bool _read_packet(let::network::byte_buffer &data);

        [[nodiscard]] let::var_int _read_var_int();

        std::atomic<bool> _processing;
        std::thread _processing_thread;

        std::int32_t _compression_threshold;

        connection_status _status;
        let::network::socket _server_socket;

        let::network::byte_buffer _incoming;
        let::network::byte_buffer _outgoing;
    };
}
