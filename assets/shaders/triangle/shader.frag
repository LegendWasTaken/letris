#version 440

layout (location = 0) out vec4 frag_colour;

uniform vec3 tint;

void main()
{
    frag_colour = vec4(vec3(0.2, 0.2, 0.4), 1);
}