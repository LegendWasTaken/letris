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

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 7.1.3
)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 7.1.3
)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.10.2
)

FetchContent_MakeAvailable(glfw glm sockpp fmt json)
