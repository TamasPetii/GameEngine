#version 460 core

layout(location = 0) in vec2 fs_in_tex;
layout(location = 0) out vec3 fs_out_col;

uniform sampler2D u_bgTexture;

void main()
{
    fs_out_col = texture(u_bgTexture, fs_in_tex).xyz;
}