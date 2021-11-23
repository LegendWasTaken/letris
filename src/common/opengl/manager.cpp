#include "manager.h"

std::uint64_t let::opengl::manager::create_program(const std::initializer_list<std::string> &shaders) {
    ZoneScopedN("opengl::create_program");
    if (shaders.size() > 2)
        LET_EXCEPTION(exception::source_type::opengl, "Attempted to create a program with {} shaders, max is 4",
                      shaders.size());

    // Max 2 shaders per program
    auto compiled_shaders = std::array<std::optional<std::uint32_t>, 4>();

    for (int i = 0; i < shaders.size(); i++)
        compiled_shaders[i] = _create_shader(*(shaders.begin() + i));

    _programs.push_back(_create_program(compiled_shaders));

    // Yes, the ID is the index, do you have a problem?
    return _programs.size() - 1;
}

let::opengl::manager::~manager() {
    ZoneScopedN("manager::dtor");
    // Manual freeing go BRRRRRRRRRRRRRRRRRRRRRR
    for (auto prog : _programs) {
        glDeleteProgram(prog.handle);

        for (const auto &shader : prog.shaders)
            if (shader.has_value())
                glDeleteShader(shader.value());
    }
}

void let::opengl::manager::bind(std::uint64_t program) {
    _current_program = program;
    glUseProgram(_programs[program].handle);
}

std::uint32_t let::opengl::manager::_create_shader(const std::string &path) {
    ZoneScopedN("opengl::internal::create_shader");

    if (!std::filesystem::exists(std::string(LETRIS_ASSET_PATH) + path))
        LET_EXCEPTION(exception::source_type::opengl, "Shader path does not exist: [{}]", path);

    const auto source = let::read_file(std::string(LETRIS_ASSET_PATH) + path);

    auto type = GLuint();
    if (path.ends_with(".vert"))
        type = GL_VERTEX_SHADER;
    else if (path.ends_with(".frag"))
        type = GL_FRAGMENT_SHADER;
    else if (path.ends_with(".comp"))
        type = GL_COMPUTE_SHADER;
    else
        LET_EXCEPTION(exception::source_type::opengl, "Couldn't figure out shader type from path: [{}]", path);

    const auto handle = glCreateShader(type);

    const auto source_str = source.c_str();
    glShaderSource(handle, 1, &source_str, nullptr);
    glCompileShader(handle);

    auto success = std::int32_t(0);
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

    if (!success) {
        auto log = std::array<char, 513>();
        glGetShaderInfoLog(handle, 512, nullptr, log.data());
        LET_EXCEPTION(exception::source_type::opengl, "Error compiling shader:\n{}", log.data());
    }

    return handle;
}

let::opengl::manager::program
let::opengl::manager::_create_program(const std::array<std::optional<std::uint32_t>, 4> &shaders) {

    ZoneScopedN("opengl::internal::create_program");
    const auto handle = glCreateProgram();

    for (const auto shader : shaders)
        if (shader.has_value())
            glAttachShader(handle, shader.value());

    glLinkProgram(handle);

    auto success = int(0);
    auto log = std::array<char, 513>();
    glGetProgramiv(handle, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(handle, 512, nullptr, log.data());
        LET_EXCEPTION(exception::source_type::opengl, "Error linking program:\n{}", log.data());
    }

    return {
        .handle = handle,
        .shaders = shaders
    };
}

void let::opengl::manager::uniform(const std::string &name, float value) {
    glUniform1f(_uniform_location(name), value);
}

void let::opengl::manager::uniform(const std::string &name, glm::vec2 value) {
    glUniform2f(_uniform_location(name), value.x, value.y);
}

void let::opengl::manager::uniform(const std::string &name, glm::vec3 value) {
    glUniform3f(_uniform_location(name), value.x, value.y, value.z);
}

void let::opengl::manager::uniform(const std::string &name, glm::vec4 value) {
    glUniform4f(_uniform_location(name), value.x, value.y, value.z, value.w);
}

void let::opengl::manager::uniform(const std::string &name, int value) {
    glUniform1i(_uniform_location(name), value);
}

void let::opengl::manager::uniform(const std::string &name, glm::ivec2 value) {
    glUniform2i(_uniform_location(name), value.x, value.y);
}

void let::opengl::manager::uniform(const std::string &name, glm::ivec3 value) {
    glUniform3i(_uniform_location(name), value.x, value.y, value.z);
}

void let::opengl::manager::uniform(const std::string &name, glm::ivec4 value) {
    glUniform4i(_uniform_location(name), value.x, value.y, value.z, value.w);
}

void let::opengl::manager::uniform(const std::string &name, glm::mat4 value) {
    glUniformMatrix4fv(_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
}

std::int32_t let::opengl::manager::_uniform_location(const std::string &name) {
    return glGetUniformLocation(_programs[_current_program].handle, name.c_str());
}
