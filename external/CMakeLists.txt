option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_INSTALL "" OFF)
option(GLFW_BUILD_EXAMPLES "" OFF)

include(FetchContent)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.3.2
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 0.9.9.8
)

FetchContent_Declare(
        sockpp
        GIT_REPOSITORY https://github.com/fpagliughi/sockpp
        GIT_TAG v0.7
)

option(ZLIB_ENABLE_TESTS "" OFF)
option(WITH_NATIVE_INSTRUCTIONS "" ON)
FetchContent_Declare(
        zlib
        GIT_REPOSITORY https://github.com/zlib-ng/zlib-ng
        GIT_TAG 2.0.2
)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.10.2
)

FetchContent_Declare(
        daw
        GIT_REPOSITORY https://github.com/beached/header_libraries
        GIT_TAG v1.29.7
)

option(QUILL_FMT_EXTERNAL "" ON)
FetchContent_Declare(
        fc_spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG        v1.9.2
)

FetchContent_MakeAvailable(glfw glm sockpp json zlib daw fc_spdlog)
