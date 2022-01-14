cmake_minimum_required(VERSION 3.17)
project(Letris)

set(CMAKE_PREFIX_PATH "C:/Program Files (x86)/my_project")

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
option(LETRIS_TESTS "" OFF)

if (LETRIS_USE_RELATIVE_ASSET_PATH)
    message("Using relative asset path")
    set(LETRIS_ASSET_PATH_VAR "./assets/")
else()
    message("Using none-relative asset path")
    set(LETRIS_ASSET_PATH_VAR "${CMAKE_SOURCE_DIR}/assets/")
endif()


add_subdirectory(external)

add_library(Letris_lib
        # Glad.c
        external/glad/glad.c

        # Network
        src/network/socket.cpp
        src/network/query_manager.cpp
        src/network/game_network.cpp
        src/network/nbt.cpp

        # World
        src/world/chunk/chunk.cpp
        src/world/world.cpp

        # Game
        src/game/game.cpp

        # Window
        src/window/window.cpp

        # Util
        src/util/luts.h
        src/util/files.h

        # Ui
        src/ui/user_input.cpp
        src/ui/ultralight_filesystem.cpp

        # Common
        src/common/network/byte_buffer.cpp
        src/common/network/packets.cpp
        src/common/network/types.cpp
        src/common/logical_devices.cpp
        src/common/opengl/manager.cpp
        src/common/task_executor.cpp

        # Tracy
        src/tracy/TracyClient.cpp
        src/common/exception.cpp

        # Renderer
        src/renderer/renderer.cpp src/common/timer.cpp src/common/timer.h src/world/block.h src/common/rolling_buffer.h)

if (LET_USE_TRACY)
    message("Tracy has been enabled")
    set(LET_TRACY_MACRO -DTRACY_ENABLE)
else ()
    set(LET_TRACY_MACRO "")
endif ()

target_link_directories(Letris_lib PUBLIC ${CMAKE_SOURCE_DIR}/assets/libs/)
target_include_directories(Letris_lib PUBLIC external src)
target_link_libraries(Letris_lib PUBLIC glm glfw sockpp-static nlohmann_json::nlohmann_json zlib AppCore Ultralight UltralightCore WebCore daw::daw-header-libraries spdlog::spdlog)
target_compile_definitions(Letris_lib PUBLIC GLFW_INCLUDE_NONE ${LET_TRACY_MACRO} -DLETRIS_ASSET_PATH="${LETRIS_ASSET_PATH_VAR}" NOMINMAX)

add_executable(Letris src/main.cpp src/common/thread/locked_resource.h src/world/entities/components.h src/bridge/render_data.cpp src/bridge/render_data.h)
target_link_libraries(Letris PUBLIC Letris_lib)

if (LETRIS_TESTS)
    add_subdirectory(tests)
endif()
