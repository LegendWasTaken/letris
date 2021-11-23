#version 460

layout (location = 0) out vec4 frag_colour;

in vec3 c_col;

uniform vec3 tint;

void main()
{
//    vec3 sun_dir = normalize(vec3(0, 1, 0.2));
//    float lighting = dot(sun_dir)

    frag_colour = vec4(c_col, 1);
}