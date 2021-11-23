#version 460

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;


out vec3 c_col;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(pos, 1.0);
    c_col = col;
}