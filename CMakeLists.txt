cmake_minimum_required(VERSION 3.17)
project(Letris)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC" AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Using clang-cl options (MSVC interface)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR- /arch:AVX /MT /W4 /EHa /EHs")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using Clang/GCC compiler options")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    if ((CMAKE_BUILD_TYPE MATCHES Release) OR (CMAKE_BUILD_TYPE MATCHES RelWithDbgInfo))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
    endif ()
endif ()

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
        src/network/encoder.h src/util/byte_buffer.h)

target_include_directories(Letris PRIVATE external src)
target_link_libraries(Letris glm glfw sockpp-static fmt nlohmann_json::nlohmann_json)