#include <game/game.h>

#include <iostream>

int main() {
    /*
     * Some of the libraries I use aren't perfect, and require some type of initialization
     * at the start of sorts. While I could abstract this into the classes, and in the ctor,
     * I would rather do it once here, in a place that is very obvious.
     *
     * Current libraries used and why static initialization is required
     * - Sockets, windows / unix stuff
     * - Ultralight, follows singleton pattern. Abstraction removes this pattern, but a result
     *               is needing to have this static initialization here
     * - GLFW / GLAD, Need static initialization, problem is GLAD needs to be initialized
     *                after the first window is created. So there is some code needed to be
     *                inside of the class logic
     */

    // Initialize the socket library
    sockpp::socket_initializer sockInit {};

    // Initialize GLFW and GLAD
    let::window_init windowInit {};

    // Initialize Ultralight and create a renderer to use
    let::ultralight_init ultralightInit {};

    auto window = let::window(1920, 1080, "Test");
    auto ultralight_renderer = ultralight::Renderer::Create();
    auto user_input_renderer = let::user_input_renderer(&ultralight_renderer, {1920, 1080});

    auto main_menu = let::main_menu();
    auto graphics_menu = let::graphics_menu();

    main_menu.on_multiplayer_click([]{
        std::cout << "Multiplayer Button Clicked" << std::endl;
    });

    main_menu.on_graphics_click([&user_input_renderer, &graphics_menu]{
        user_input_renderer.use(&graphics_menu);
    });

    main_menu.on_sound_click([]{
        std::cout << "Sound Button Clicked" << std::endl;
    });

    graphics_menu.on_main_menu_click([&user_input_renderer, &main_menu]{
        user_input_renderer.use(&main_menu);
    });

    user_input_renderer.use(&main_menu);

    auto texture = GLuint();
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    window.set_texture_callback([&window, &user_input_renderer, texture](){
        const auto mouse = window.mouse();
        const auto keyboard = window.keyboard();

        user_input_renderer.update({
            .mouse = mouse,
            .keyboard = keyboard
        });

        user_input_renderer.render();
        user_input_renderer.read_into(texture);
        return texture;
    });

    while (!window.should_close())
        window.display_frame();


//    auto game_network = let::network::game();
//    game_network.connect("127.0.0.1", 25565);

//    while (true)
//        game_network._process();

//    auto game_network = let::network::game();

//    auto game = let::game_builder()
//            .with_network(game_network)
//            .with_window(window)
//            .with_ui_renderer(user_input_renderer)
//            .build();
//
//    game.start();

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
