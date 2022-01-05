#include <game/game.h>

int main() {
    ZoneScopedN("main");

    auto pool = let::thread_pool(8);

    let::block block(0);

    sockpp::socket_initializer sockInit {};
    let::window_init windowInit {};
    let::ultralight_init ultralightInit {};

    try {
        spdlog::set_level(spdlog::level::debug);
        spdlog::info("Starting");

        auto opengl_manager = let::opengl::manager();

        auto game_network = let::network::game();
        auto window = let::window(1920, 1080, "Test", &opengl_manager);
        auto ultralight_renderer = ultralight::Renderer::Create();
        auto user_input_renderer = let::user_input_renderer(&ultralight_renderer, {1920, 1080});
        auto server_querier = let::network::query();
        auto renderer = let::renderer(1920, 1080, &opengl_manager);

        auto render_cache = let::bridge::render_data_cache(&opengl_manager);

        auto game = let::game_builder()
                .with_network(game_network)
                .with_window(window)
                .with_ui_renderer(user_input_renderer)
                .with_server_querier(server_querier)
                .with_renderer(renderer)
                .with_render_cache(render_cache)
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
