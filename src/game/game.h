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
#include <common/timer.h>
#include <common/thread/locked_resource.h>

#include <bridge/render_data.h>

#include <glm/gtx/euler_angles.hpp>

namespace let {
    class game {
    public:
        void start();

        game(let::network::game *game_network, let::window *window, let::user_input_renderer *ui_renderer,
             let::network::query *server_querier, let::renderer *renderer, let::bridge::render_data_cache *render_data_cache);

    private:
        let::network::game *_game_network;
        let::window *_window;
        let::user_input_renderer *_ui_renderer;
        let::network::query *_server_querier;
        let::renderer *_renderer;
        std::optional<let::world> _world;

        let::bridge::render_data_cache *_render_cache;

        bool _running;

        std::string _server_to_join;

        glm::vec3 _world_pos {};
        glm::ivec2 _previous_mouse_pos;
        glm::vec2 _rotation;

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

        void _tick(double dt);
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

        [[nodiscard]] game_builder &with_render_cache(let::bridge::render_data_cache &cache);

        [[nodiscard]] let::game build() const;

    private:
        let::network::game *_game_network = nullptr;

        let::window *_window = nullptr;

        let::user_input_renderer *_ui_renderer = nullptr;

        let::network::query *_server_querier = nullptr;

        let::renderer *_renderer = nullptr;

        let::bridge::render_data_cache *_render_data_cache = nullptr;
    };
}

#endif //LETRIS_GAME_H
