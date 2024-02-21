#version 460 core

layout(location = 0) out vec4 fs_out_col;

uniform vec3 u_color;

void main()
{
    fs_out_col = vec4(u_color, 1);
}