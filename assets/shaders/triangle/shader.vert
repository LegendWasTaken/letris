#version 460

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec4 col;

out vec3 c_col;
out vec3 c_normal;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(pos.xyz, 1.0);
    c_normal = normal.xyz;
    c_col = col.xyz;
}