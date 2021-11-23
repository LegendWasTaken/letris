#ifndef LETRIS_RENDERER_H
#define LETRIS_RENDERER_H

#include <common/opengl/manager.h>

#include <common/exception.h>

#include <glm/gtc/matrix_transform.hpp>

namespace let {
    class renderer {
    public:
        explicit renderer(std::uint16_t width, std::uint16_t  height, let::opengl::manager *gl_manager);

        struct render_data {
            glm::vec3 offset;
            glm::mat4 rotation;
            std::span<const glm::vec3> vertices;
            std::span<const std::uint32_t> indices;
        };

        [[nodiscard]] std::uint32_t render(const render_data &data);

    private:
        struct {
            GLuint handle;
            GLuint texture;
            GLuint depth;
            GLuint rbo;
        } _framebuffer {};

        struct {
            std::uint32_t vbo;
            std::uint32_t vao;
            std::uint32_t ebo;
        } _tri;

        std::uint32_t _triangle_program;

        glm::ivec2 _resolution;

        let::opengl::manager *_gl_manager;
    };
}

#endif //LETRIS_RENDERER_H
