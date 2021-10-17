#ifndef LETRIS_GAME_H
#define LETRIS_GAME_H

#include <network/game_network.h>
#include <network/query_manager.h>
#include <renderer/renderer.h>
#include <window/window.h>
#include <world/world.h>
#include <ui/user_input.h>

#include <tracy/Tracy.hpp>

#include <common/exception.h>
#include <common/thread/locked_resource.h>

namespace let {
    class game {
    public:
        void start();

        game(let::network::game *game_network, let::window *window, let::user_input_renderer *ui_renderer,
             let::network::query *server_querier, let::renderer *renderer, let::world *world);

    private:
        let::network::game *_game_network;
        let::window *_window;
        let::user_input_renderer *_ui_renderer;
        let::network::query *_server_querier;
        let::renderer *_renderer;
        let::world *_world;

        bool _running;

        std::string _server_to_join;

        struct {
            let::main_menu main;
            let::graphics_menu graphics;
            let::multiplayer_menu multiplayer;
        } _menus;

        struct {
            struct {
                GLuint target = ~0;
                GLuint render_target = ~0;
            } texture;
        } _gpu;

        glm::ivec2 _current_resolution;

        void _initialize_menus();

        void _create_gpu_resources();

        void _world_tick();
    };
}

namespace let {
    class game_builder {
    public:
        game_builder() = default;

        [[nodiscard]] game_builder &with_network(let::network::game &game_network);

        [[nodiscard]] game_builder &with_window(let::window &window);

        [[nodiscard]] game_builder &with_ui_renderer(let::user_input_renderer &ui_renderer);

        [[nodiscard]] game_builder &with_server_querier(let::network::query &server_querier);

        [[nodiscard]] game_builder &with_renderer(let::renderer &renderer);

        [[nodiscard]] game_builder &with_world(let::world &world);

        [[nodiscard]] let::game build() const;

    private:
        let::network::game *_game_network = nullptr;

        let::window *_window = nullptr;

        let::user_input_renderer *_ui_renderer = nullptr;

        let::network::query *_server_querier = nullptr;

        let::renderer *_renderer = nullptr;

        let::world *_world;
    };
}

#endif //LETRIS_GAME_H
