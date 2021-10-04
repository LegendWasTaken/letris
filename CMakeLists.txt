cmake_minimum_required(VERSION 3.17)
project(Letris)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC" AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Using clang-cl options (MSVC interface)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR- /MT /W4 /EHa /EHs")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using Clang/GCC compiler options")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    if ((CMAKE_BUILD_TYPE MATCHES Release) OR (CMAKE_BUILD_TYPE MATCHES RelWithDbgInfo))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
    endif ()
endif ()

option(LET_USE_TRACY "" OFF)
option(LETRIS_USE_RELATIVE_ASSET_PATH "" OFF)

if (LETRIS_USE_RELATIVE_ASSET_PATH)
    message("Using relative asset path")
    set(LETRIS_ASSET_PATH_VAR "./assets/")
else()
    message("Using none-relative asset path")
    set(LETRIS_ASSET_PATH_VAR "${CMAKE_SOURCE_DIR}/assets/")
endif()


add_subdirectory(external)

add_executable(Letris
        # Glad.c
        external/glad/glad.c

        # Main
        src/main.cpp

        # Network
        src/network/byte_buffer.cpp
        src/network/byte_buffer.h
        src/network/socket.cpp
        src/network/socket.h
        src/network/decoder.h
        src/network/types.h
        src/network/types.cpp
        src/network/packets.h
        src/network/packets.cpp
        src/network/encoder.h
        src/network/query_manager.cpp
        src/network/query_manager.h
        src/network/game_network.cpp
        src/network/game_network.h
        src/network/nbt.cpp
        src/network/nbt.h

        # World
        src/world/chunk/chunk.cpp
        src/world/chunk/chunk.h

        # Game
        src/game/game.cpp
        src/game/game.h

        # Window
        src/window/window.cpp
        src/window/window.h

        # Util
        src/util/opengl.h
        src/util/luts.h
        src/util/files.h

        # Ui
        src/ui/user_input.cpp
        src/ui/user_input.h
        src/ui/ultralight_filesystem.cpp
        src/ui/ultralight_filesystem.h

        # Common
        src/common/logical_devices.cpp
        src/common/logical_devices.h

        # Tracy
        src/tracy/TracyClient.cpp
        )

if (LET_USE_TRACY)
    message("Tracy has been enabled")
    set(LET_TRACY_MACRO -DTRACY_ENABLE)
else ()
    set(LET_TRACY_MACRO "")
endif ()

target_link_directories(Letris PUBLIC ${CMAKE_SOURCE_DIR}/assets/libs/)
target_include_directories(Letris PRIVATE external src)
target_link_libraries(Letris glm glfw sockpp-static fmt nlohmann_json::nlohmann_json zlib AppCore Ultralight UltralightCore WebCore)
target_compile_definitions(Letris PRIVATE GLFW_INCLUDE_NONE ${LET_TRACY_MACRO} -DLETRIS_ASSET_PATH="${LETRIS_ASSET_PATH_VAR}")
