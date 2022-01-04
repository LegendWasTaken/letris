#include "game.h"

#include <bridge/render_data.h>

let::game::game(let::network::game *game_network, let::window *window, let::user_input_renderer *ui_renderer,
                let::network::query *server_querier, let::renderer *renderer, let::bridge::render_data_cache *render_data_cache)
        : _game_network(game_network), _window(window), _ui_renderer(ui_renderer), _server_querier(server_querier),
          _renderer(renderer), _current_resolution(window->resolution()), _render_cache(render_data_cache) {
    ZoneScopedN("game::constructor");
    _initialize_menus();
    _create_gpu_resources();
}

void let::game::start() {
    ZoneScopedN("game::start");

    _running = true;

    auto timer = let::timer();

    while (!_window->should_close() && _running) {
        timer.frame_start();
        _tick(timer.since_last_frame());
        timer.frame_end();
    }
}

void let::game::_initialize_menus() {
    ZoneScopedN("game::initialize_menus");
    // Setup the main menu buttons
    _menus.main.on_multiplayer_click([this] {
        auto server_to_query = std::vector<let::network::query::queryable_server>();
        server_to_query.push_back(network::query::queryable_server{
                .port = 25565,
                .address = "localhost"
        });
//        server_to_query.push_back(network::query::queryable_server{
//                .port = 25565,
//                .address = "mc.hypixel.net"
//        });

        _server_querier->query_servers(server_to_query);
        _ui_renderer->use(&_menus.multiplayer);
    });

    _menus.main.on_graphics_click([this] {
        _ui_renderer->use(&_menus.graphics);
    });

    _menus.main.on_sound_click([] {
        std::cout << "Sound Button Clicked" << std::endl;
    });

    _menus.graphics.on_main_menu_click([this] {
        _ui_renderer->use(&_menus.main);
    });

    _menus.multiplayer.on_main_menu_click([this] {
        _ui_renderer->use(&_menus.main);
    });

    _menus.multiplayer.on_server_data_request([this](ultralight::JSArgs args) -> ultralight::JSValue {
        const auto servers = _server_querier->query_status();
        auto json = nlohmann::json();

        for (const auto &server : servers) {
            auto server_json = nlohmann::json();

            server_json["connected"] = server.info.can_connect;
            server_json["description"] = server.info.description.text;
            server_json["target"] = server.target.address;

            json.emplace_back(server_json);
        }

        auto json_str = nlohmann::to_string(json);

        return ultralight::JSValue(nlohmann::to_string(json).c_str());
    });

    _menus.multiplayer.on_server_click([this](ultralight::JSArgs args) {
        const auto server_name = args[0].ToString();

        // Getting it to go from a JSString to std::string is a slight pain
        const auto opaque_string = server_name.operator OpaqueJSString *();
        const auto str_len = JSStringGetLength(opaque_string);
        auto server_name_str = std::string();
        server_name_str.resize(str_len);
        JSStringGetUTF8CString(opaque_string, server_name_str.data(), str_len);

        _server_to_join = server_name_str;

        spdlog::debug("Clicked server: {}", _server_to_join);
    });

    _ui_renderer->use(&_menus.main);
}

void let::game::_create_gpu_resources() {
    ZoneScopedN("game::create_gpu_resources");
    glGenTextures(1, &_gpu.texture.target);
    glBindTexture(GL_TEXTURE_2D, _gpu.texture.target);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _current_resolution.x, _current_resolution.y, 0, GL_BGRA, GL_UNSIGNED_BYTE,
                 nullptr);
}

void let::game::_tick(double dt) {
    ZoneScopedN("game::tick");
    if (_game_network->status() == network::game::connection_status::connected ||
        _game_network->status() == network::game::connection_status::connecting) {
        _game_network->_process();

        if (_game_network->status() == network::game::connection_status::connected) {
            if (!_world.has_value())
                _world = let::world();

            static auto out_going = let::network::byte_buffer();
            auto incoming = _game_network->incoming();
            _world->process_packets(incoming, out_going);
            _game_network->send_data(out_going);
        }

    } else {
        auto target_server = _server_to_join;

        if (!target_server.empty()) {
            spdlog::info("Attempting to join: {}", target_server);

            // Todo: Custom ports
            _game_network->connect(target_server, 25565);

            _server_to_join.clear();
        }
    }

    const auto mouse = _window->mouse();
    const auto keyboard = _window->keyboard();

    _ui_renderer->update({
                                 .mouse = mouse,
                                 .keyboard= keyboard
                         });

    if (_world.has_value()) {
        _window->mouse().hide();

        constexpr auto speed = 200.0f;

        auto movement_direction = glm::vec3();

        if (keyboard.is_key_down(logical::keyboard::key_code::key_w))
            movement_direction.z += -1;

        if (keyboard.is_key_down(logical::keyboard::key_code::key_s))
            movement_direction.z += 1;

        if (keyboard.is_key_down(logical::keyboard::key_code::key_a))
            movement_direction.x += -1;

        if (keyboard.is_key_down(logical::keyboard::key_code::key_d))
            movement_direction.x += 1;

        if (keyboard.is_key_down(logical::keyboard::key_code::key_left_shift))
            movement_direction.y += -1;

        if (keyboard.is_key_down(logical::keyboard::key_code::space))
            movement_direction.y += 1;

        // calculate mouse movement
        const auto pos = _window->mouse().position();
        const auto delta = pos - _previous_mouse_pos;
        _previous_mouse_pos = pos;

        _rotation.x += delta.y * 0.0004;
        _rotation.y += delta.x * 0.0004;

        const auto rotation = glm::eulerAngleXY(_rotation.x, _rotation.y);
        // Todo: move this logic of moving into the world
        _world_pos += glm::vec3(glm::inverse(rotation) * glm::vec4(movement_direction, 1.0f)) * glm::vec3(1.1);

        const auto world_pos = _world_pos + _world.value().world_pos() + glm::vec3(0, 500, 0);

        auto render_data = let::bridge::render_data(_world.value(), *_render_cache);
        auto chunk_data = render_data.data();

        _gpu.texture.render_target = _renderer->render({
                                                               .offset = world_pos,
                                                               .rotation = rotation,
                                                               .positions = chunk_data.positions,
                                                               .faces = chunk_data.faces,
                                                               .indirects = chunk_data.indirects
                                                       });
    } else {
        _window->mouse().show();
        _ui_renderer->render();
        _ui_renderer->read_into(_gpu.texture.target);
    }

    _window->display_frame({
                                   .gui = _gpu.texture.target,
                                   .rendered = _gpu.texture.render_target,
                                   .has_world = _world.has_value()
                           });
}

let::game_builder &let::game_builder::with_network(let::network::game &game_network) {
    _game_network = &game_network;
    return *this;
}

let::game_builder &let::game_builder::with_window(let::window &window) {
    _window = &window;
    return *this;
}

let::game_builder &let::game_builder::with_ui_renderer(let::user_input_renderer &ui_renderer) {
    _ui_renderer = &ui_renderer;
    return *this;
}

let::game_builder &let::game_builder::with_server_querier(let::network::query &server_querier) {
    _server_querier = &server_querier;
    return *this;
}

let::game_builder &let::game_builder::with_renderer(let::renderer &renderer) {
    _renderer = &renderer;
    return *this;
}

let::game_builder &let::game_builder::with_render_cache(let::bridge::render_data_cache &cache) {
    _render_data_cache = &cache;
    return *this;
}

let::game let::game_builder::build() const {
    return let::game(_game_network, _window, _ui_renderer, _server_querier, _renderer, _render_data_cache);
}
