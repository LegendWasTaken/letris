#include <game/game.h>

#include <common/exception.h>

#include <spdlog/spdlog.h>

int main() {
    ZoneScopedN("main");

    sockpp::socket_initializer sockInit {};
    let::window_init windowInit {};
    let::ultralight_init ultralightInit {};

    try {
        // Hey, just don't write any bugs
        spdlog::info("Starting");

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
    } catch (const let::exception &exception) {
        const auto source = exception.where();
        spdlog::critical("Experienced an unrecoverable exception: "
                         "\n\tReason: {}"
                         "\n\tType: {}"
                         "\n\tLocation: {}:{}",
                         exception.what(), source.type, source.file, source.line);
    } catch (const std::exception &exception) {
        spdlog::critical("Experienced a C++ exception: "
                         "\n\tReason: {}",
                         exception.what());
    } catch (...) {
        spdlog::critical("Experienced an unknown exception, the dirt gods aren't happy");
    }
}
