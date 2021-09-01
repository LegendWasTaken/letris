#include <network/game_network.h>

int main() {
    sockpp::socket_initializer sockInit;

    auto game_network = let::network::game();
    game_network.connect("127.0.0.1", 25565);

    while (true)
        game_network._process();

    /*

    auto network_manager = let::network_manager();

    auto world = let::world();

    auto input_handler = let::input_handler();

    auto window = let::window();

    auto renderer = let::renderer();

    auto game = let::game_builder()
        .with_network_manager(network_manager)
        .with_world(world)
        .with_input_handler(input_handler)
        .with_window(window)
        .with_renderer(renderer).build();

    game.start();
     */

//    auto socket = let::network::socket("localhost", 25565);

//    auto test_buffer = let::network::byte_buffer(1024);

//    let::packets::write<let::packets::state::handshaking>::handshake(test_buffer, let::packets::handshake_state::login, 25565, "127.0.0.1");
//    auto result = socket.send(test_buffer);

//    let::packets::write<let::packets::state::login>::login_start(test_buffer, "Test");
//    result = socket.send(test_buffer);

//    while (!socket.receive(test_buffer));

    return 0;
}
