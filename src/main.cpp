#include <game/game.h>

int main() {
    sockpp::socket_initializer sockInit {};

    let::window_init windowInit {};
    let::ultralight_init ultralightInit {};

    auto game_network = let::network::game();

    auto window = let::window(1920, 1080, "Test");

    auto ultralight_renderer = ultralight::Renderer::Create();
    auto user_input_renderer = let::user_input_renderer(&ultralight_renderer, {1920, 1080});

    auto server_querier = let::network::query();

    auto game = let::game_builder()
            .with_network(game_network)
            .with_window(window)
            .with_ui_renderer(user_input_renderer)
            .with_server_querier(server_querier)
            .build();

    game.start();
}
