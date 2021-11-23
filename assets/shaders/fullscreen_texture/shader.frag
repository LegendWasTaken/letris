#version 460

layout (location = 0) out vec4 frag_colour;

layout (location = 0) in vec2 tex_coord;

layout (binding = 0) uniform sampler2D gui;

layout (binding = 1) uniform sampler2D rendered;

layout (location = 0) uniform int has_world;

void main()
{
    vec2 non_flipped = vec2(tex_coord.x, tex_coord.y);
    vec2 flipped = vec2(non_flipped.x, non_flipped.y * -1 + 1);

    if (bool(has_world))
        frag_colour = texture(rendered, non_flipped);
    else
        frag_colour = texture(gui, flipped).zyxw;
}
