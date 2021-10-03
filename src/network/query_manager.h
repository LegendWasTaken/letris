#pragma once

#include <network/socket.h>
#include <network/packets.h>

#include <atomic>
#include <vector>
#include <optional>

namespace let::network {
    class query {
    public:
        query() = default;

        query(const query &other) = delete;

        query(query &&other) = delete;

        query &operator=(const query &other) = delete;

        query &operator=(query &&other) = delete;

        struct queryable_server {
            std::uint16_t port;
            std::string address;
        };

        /// \brief Starts querying servers for their information, sets mode to [query]
        /// \param servers The servers to be queried
        void query_servers(const std::vector<queryable_server> &servers);

        struct server_info {
            bool can_connect = false;

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

        struct queryable_server_info {
            queryable_server target;
            server_info info;
        };

        /// \brief Retrieve the data from the queried servers, might not be available
        /// \return A list of optional server data
        [[nodiscard]] std::vector<queryable_server_info> query_status();

    private:
        struct queryable_server_socket {
            queryable_server target{};
            std::optional<server_info> info;
            std::unique_ptr<let::network::socket> socket;
        };

        std::vector<queryable_server_socket> _sockets;
    };
}
