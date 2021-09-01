#include <network/socket.h>

int main() {
    sockpp::socket_initializer sockInit;
    auto socket = let::network::socket("localhost", 25565);

    auto test_buffer = let::network::byte_buffer(1024);

    let::packets::write<let::packets::state::handshaking>::handshake(test_buffer, let::packets::handshake_state::status, 25565, "127.0.0.1");
    auto result = socket.send(test_buffer);
    let::packets::write<let::packets::state::status>::request(test_buffer);
    result = socket.send(test_buffer);

    while (!socket.receive(test_buffer))
    {
    }

    auto response = let::packets::read<let::packets::state::status>::response(test_buffer);

    let::packets::write<let::packets::state::status>::ping(test_buffer, 1024);
    result = socket.send(test_buffer);

    while (!socket.receive(test_buffer))
    {
    }

    auto pong = let::packets::read<let::packets::state::status>::pong(test_buffer);

    return 0;
}
