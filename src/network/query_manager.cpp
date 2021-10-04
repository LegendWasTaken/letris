#include "query_manager.h"

void let::network::query::query_servers(const std::vector<queryable_server> &servers) {
    _sockets.clear();

    auto buffer = let::network::byte_buffer(1024);

    for (const auto &server : servers) {
        auto query = queryable_server_socket();
        query.target = {.port = server.port, .address = server.address};

        query.socket = std::make_unique<let::network::socket>(server.address, server.port);
        if (query.socket->connected())
        {
            // Fire off the code required to send
            let::packets::write<let::packets::state::handshaking>::handshake(buffer, let::packets::handshake_state::status,
                                                                             server.port, server.address.c_str());
            query.socket->send(buffer);
            let::packets::write<let::packets::state::status>::request(buffer);
            query.socket->send(buffer);
        } else {
            query.socket = nullptr;
        }

        _sockets.emplace_back(std::move(query));
    }
}

std::vector<let::network::query::queryable_server_info> let::network::query::query_status() {
    // Process the incoming sockets
    auto servers = std::vector<let::network::query::queryable_server_info>();

    auto buffer = let::network::byte_buffer(10240);
    for (auto &sock : _sockets) {
        auto info = query::queryable_server_info();
        info.target = sock.target;

        if (sock.socket == nullptr || !sock.socket->connected()) {
            info.info.can_connect = false;
        }
        else if (!sock.info.has_value()) {
            try {
                buffer.clear();

                // The maximum size of a packet header is 6 bytes
                sock.socket->receive(buffer, 6);

                auto length = let::var_int();
                auto packet_id = let::var_int();
                let::network::decoder::read(buffer, length);
                let::network::decoder::read(buffer, packet_id);
                buffer.step_back(length.length() + packet_id.length());

                /***
                 * A quick breakdown of what's going on here,
                 * we read 6 no matter what.
                 *
                 * First, we calculate how much we "overshot"
                 * eg, if the header was only 2 bytes, we overshot by 4
                 *
                 * Then, to calculate how much we still need to read
                 * We take the packet length, take away the size of the packet id, and the overshoot
                 */

                const auto overshot = 6 - (length.length() + packet_id.length());
                const auto packet_data_length = int(length) - packet_id.length() - overshot;

                sock.socket->receive(buffer, packet_data_length);


                if (buffer.size() > 0) {
                    const auto server_data = let::packets::read<let::packets::state::status>::response(buffer);

                    info.info.can_connect = true;
                    info.info.version.name = server_data.version.name;
                    info.info.players.players_max = server_data.players.players_max;
                    info.info.players.online = server_data.players.online;
                    info.info.description.text = server_data.description.text;
                    sock.info = info.info;
                }
            } catch (...) {
                info.info.can_connect = false;
                sock.socket->disconnect();
            }
        } else {
            info.info = sock.info.value();
        }
        servers.push_back(info);
    }

    return servers;
}
