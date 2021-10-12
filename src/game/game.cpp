#include "game.h"

let::game::game(let::network::game *game_network, let::window *window, let::user_input_renderer *ui_renderer,
                let::network::query *server_querier, let::renderer *renderer, let::world *world)
        : _game_network(game_network), _window(window), _ui_renderer(ui_renderer), _server_querier(server_querier),
          _renderer(renderer), _current_resolution(window->resolution()), _world(world) {
    ZoneScopedN("game::constructor");
    _initialize_menus();
    _create_gpu_resources();
}

void let::game::start() {
    ZoneScopedN("game::start");
    _window->set_texture_callback([this]() {
        const auto mouse = _window->mouse();
        const auto keyboard = _window->keyboard();

        _ui_renderer->update({
                                     .mouse = mouse,
                                     .keyboard= keyboard
                             });

        _ui_renderer->render();
        _ui_renderer->read_into(_gpu.texture.target);

        return _gpu.texture.target;
    });

    _running = true;

    auto world_thread = std::thread([this]() {
        while (_running) {
            if (_game_network->status() == network::game::connection_status::connected ||
                _game_network->status() == network::game::connection_status::connecting) {
                _game_network->_process();

                const auto incoming = _game_network->incoming();

                const auto a = 5;

            } else {
                auto target_server = static_cast<std::string>(_server_to_join.value());

                if (!target_server.empty()) {
                    spdlog::info("Attempting to join: {}", target_server);

                    // Todo: Custom ports
                    _game_network->connect(target_server, 25565);


                    static_cast<std::string &>(_server_to_join.value()).clear();
                }
            }
        }
    });

    while (!_window->should_close() && _running)
        _window->display_frame();

    if (_running)
        _running = false;
    world_thread.join();
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

        static_cast<std::string&>(_server_to_join.value()) = server_name_str;
    });

    _ui_renderer->use(&_menus.main);
}

void let::game::_create_gpu_resources() {
    ZoneScopedN("window::create_gpu_resources");
    glGenTextures(1, &_gpu.texture.target);
    glBindTexture(GL_TEXTURE_2D, _gpu.texture.target);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _current_resolution.x, _current_resolution.y, 0, GL_BGRA, GL_UNSIGNED_BYTE,
                 nullptr);

//    glGenTextures(1, &_gpu.texture.render_target);
//    glBindTexture(GL_TEXTURE_2D, _gpu.texture.render_target);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _current_resolution.x, _current_resolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
//                 nullptr);
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

let::game_builder &let::game_builder::with_world(let::world &world) {
    _world = &world;
    return *this;
}

let::game let::game_builder::build() const {
    return let::game(_game_network, _window, _ui_renderer, _server_querier, _renderer, _world);
}
