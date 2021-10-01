#include "query_manager.h"

void let::network::query::query_servers(const std::vector<queryable_server> &servers) {
    _sockets.clear();

    auto buffer = let::network::byte_buffer(1024);

    for (const auto &server : servers) {
        auto query = queryable_server_socket();
        query.socket = std::make_unique<let::network::socket>(server.address, server.port);

        // Fire off the code required to send
        let::packets::write<let::packets::state::handshaking>::handshake(buffer, let::packets::handshake_state::status,
                                                                         server.port, server.address.c_str());
        query.socket->send(buffer);
        let::packets::write<let::packets::state::status>::request(buffer);
        query.socket->send(buffer);
        _sockets.emplace_back(std::move(query));
    }
}

std::vector<let::network::query::queryable_server_info> let::network::query::query_status() {
    // Process the incoming sockets
    auto servers = std::vector<let::network::query::queryable_server_info>();

    auto buffer = let::network::byte_buffer(10240);
    for (auto &sock : _sockets)
    {

        if (!sock.info.has_value())
        {
            sock.socket->receive(buffer);
            if (buffer.size() > 0) {
                const auto server_data = let::packets::read<let::packets::state::status>::response(buffer);

                auto info = query::queryable_server_info();
                info.target = sock.target;
                info.info = server_info();
                info.info->can_connect = true;
                info.info->version.name = server_data.version.name;
                info.info->players.players_max = server_data.players.players_max;
                info.info->players.online = server_data.players.online;
                info.info->description.text = server_data.description.text;
                sock.info = info.info.value();

                buffer.clear();
                servers.push_back(std::move(info));
            }
        } else {
            auto info = query::queryable_server_info();
            info.target = sock.target;
            info.info = sock.info.value();
            servers.push_back(info);
        }
    }

    return servers;
}
