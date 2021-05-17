cmake_minimum_required(VERSION 3.17)
project(Letris)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external)

add_executable(Letris
        # Glad.c
        external/glad/glad.c

        src/main.cpp
        src/game.cpp src/game.h
        src/components.h
        src/network/byte_buffer.cpp
        src/network/byte_buffer.h
        src/network/socket.cpp
        src/network/socket.h
        src/network/decoder.h
        src/network/types.h
        src/network/types.cpp
        src/network/packets.h
        src/network/packets.cpp
        src/network/encoder.h)

target_include_directories(Letris PRIVATE external src)
target_link_libraries(Letris glm glfw sockpp-static fmt)