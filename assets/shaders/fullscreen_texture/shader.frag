#version 440

layout (location = 0) out vec4 frag_colour;

layout (location = 0) in vec2 tex_coord;

layout (location = 0) uniform sampler2D tex;

void main()
{
    vec2 flipped = vec2(tex_coord.x, tex_coord.y * -1 + 1);
    vec3 colour = texture(tex, flipped).xyz;
    frag_colour = vec4(colour.rgb, 1);
}
/*
layout (location = 0) uniform sampler2D gui;
layout (location = 1) uniform sampler2D render;

uniform ivec3 use;
// X = Use GUI Y = use render Z = unused

void main()
{
    vec3 final = vec3(0);

    if (bool(use.x)) {
        vec2 flipped = vec2(tex_coord.x, tex_coord.y * -1 + 1);
        vec3 colour = texture(gui, flipped).xyz;
        final += vec3(colour.bgr * 0.5 + tint * 0.5);
    }

    if (bool(use.y)) {


        vec3 colour = texture(render, tex_coord);
        if (bool(use.x)) {
            final *= 0.5;
            colour *= 0.5;
        }

        final += colour;
    }

    frag_colour = vec4(final, 1.0);
}
*/