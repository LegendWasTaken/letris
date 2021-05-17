#include <network/socket.h>

int main()
{
  sockpp::socket_initializer sockInit;
  auto socket = let::network::socket("localhost", 25565);

  auto test_buffer = let::network::byte_buffer(1024);

  let::packets::write<let::packets::state::status>::request(test_buffer);
  const auto result = socket.send(test_buffer);
  test_buffer.clear();
  fmt::print("Result: {}", result);

  const auto wait = socket.receive(test_buffer);
  const auto test = 5;

//  glfwSetErrorCallback([](int error, const char *description) {
//    std::cerr << "GLFW Error: " << description << std::endl;
//  });
//
//  glfwInit();
//
//  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
//
//  const auto window = glfwCreateWindow(1920, 1080, "Letris", nullptr, nullptr);
//
//  glfwMakeContextCurrent(window);
//  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
//
//  while (!glfwWindowShouldClose(window))
//  {
//    glfwPollEvents();
//
//    glClearColor(1, 1, 1, 1);
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    glfwSwapBuffers(window);
//  }
  return 0;
}
