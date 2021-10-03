#version 440

layout (location = 0) out vec4 frag_colour;

layout (location = 0) in vec2 tex_coord;

layout (location = 0) uniform sampler2D tex;

layout (location = 1) uniform int has_texture;

void main()
{
    vec2 flipped = vec2(tex_coord.x, tex_coord.y * -1 + 1);
    vec3 colour = bool(has_texture) ? texture(tex, flipped).xyz : vec3(0.2, 0.3, 0.8);
    frag_colour = vec4(colour.bgr, 1);
}