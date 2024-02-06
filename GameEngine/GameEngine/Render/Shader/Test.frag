#version 460 core

layout(location = 0) in vec3 fs_in_position;
layout(location = 1) in vec3 fs_in_normal;
layout(location = 2) in vec2 fs_in_texcoord;
layout(location = 3) in flat uint fs_in_id;

layout(location = 0) out vec4 fs_out_color;
layout(location = 1) out uint fs_out_id;

uniform sampler2D tex;

void main()
{
    fs_out_color = vec4(normalize(fs_in_normal), 1.0) * texture(tex, fs_in_texcoord);
    fs_out_id = fs_in_id;
}