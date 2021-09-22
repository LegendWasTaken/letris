#version 440

layout(location = 0) out vec2 tex_coord;

void main()
{
    vec2 position = vec2(gl_VertexID % 2, gl_VertexID / 2) * 4.0 - 1;
    tex_coord = (position + 1.0) * 0.5;
    gl_Position = vec4(position, 0, 1);
}