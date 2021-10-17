#version 440

layout (location = 0) out vec4 frag_colour;

layout (location = 0) in vec2 tex_coord;

layout (binding = 0) uniform sampler2D gui;

layout (binding = 1) uniform sampler2D rendered;

void main()
{
    vec2 non_flipped = vec2(tex_coord.x, tex_coord.y);
    vec2 flipped = vec2(non_flipped.x, non_flipped.y * -1 + 1);

    vec3 final_colour = vec3(0);

    vec3 gui_sample = texture(gui, flipped).xyz;
    vec4 rendered_sample = texture(rendered, non_flipped);

    final_colour = gui_sample;
    if (rendered_sample.w != 0)
        final_colour = rendered_sample.xyz;

    frag_colour = vec4(final_colour, 1);
}
