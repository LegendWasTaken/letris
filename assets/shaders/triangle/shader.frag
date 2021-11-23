#version 460

layout (location = 0) out vec4 frag_colour;

in vec3 c_col;
in vec3 c_normal;

uniform vec3 tint;

void main()
{
    float lighting = 1.0;
    if (abs(c_normal.y) > 0.5)
        lighting = 1.0;
    else if (abs(c_normal.x) > 0.5 )
        lighting = 0.8;
    else if (abs(c_normal.z) > 0.5)
        lighting = 0.6;

    frag_colour = vec4(c_col, 1) * lighting;
}