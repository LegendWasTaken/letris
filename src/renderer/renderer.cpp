#include "renderer.h"

let::renderer::renderer(std::uint16_t width, std::uint16_t height, let::opengl::manager *gl_manager) : _gl_manager(
        gl_manager), _resolution(width, height) {
    glGenFramebuffers(1, &_framebuffer.handle);

    glGenTextures(1, &_framebuffer.texture);
    glBindTexture(GL_TEXTURE_2D, _framebuffer.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenRenderbuffers(1, &_framebuffer.rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer.handle);

    glBindTexture(GL_TEXTURE_2D, _framebuffer.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _resolution.x, _resolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _framebuffer.texture, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, _framebuffer.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _resolution.x, _resolution.y);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _framebuffer.rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LET_EXCEPTION(exception::source_type::render, "Framebuffer could not be completed");

    _triangle_program = _gl_manager->create_program({
        "shaders/triangle/shader.frag",
        "shaders/triangle/shader.vert"
    });

    auto vertices = std::array<float, 9>({
       -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        0.0,  0.5, 0.0
    });

    glGenBuffers(1, &_tri.vbo);
    glGenVertexArrays(1, &_tri.vao);

    glBindVertexArray(_tri.vao);
    glBindBuffer(GL_ARRAY_BUFFER, _tri.vbo);

    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
}

std::uint32_t let::renderer::render() {
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer.handle);
//
//    glEnable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _gl_manager->bind(_triangle_program);
    _gl_manager->uniform("tint", glm::vec3(0.2, 0.3, 0.8));
    glBindVertexArray(_tri.vao);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return _framebuffer.texture;
}
