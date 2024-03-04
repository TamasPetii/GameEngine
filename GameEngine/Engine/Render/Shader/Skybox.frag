#version 460 core

layout(location = 0) in vec3 vs_out_pos;
layout(location = 0) out vec4 fs_out_col;
uniform samplerCube skyboxTexture;

void main()
{
	fs_out_col = texture(skyboxTexture, vs_out_pos);
}