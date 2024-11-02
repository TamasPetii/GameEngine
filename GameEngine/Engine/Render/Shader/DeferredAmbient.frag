#version 460 core

layout(location = 0) in vec2 fs_in_tex;
layout(location = 0) out vec4 fs_out_col;

uniform float intensity = 0.15;
uniform sampler2D colorTexture;

void main()
{
	vec3 color = texture(colorTexture, fs_in_tex).xyz;
	vec3 ambient = intensity * color;
	fs_out_col = vec4(ambient, 1);
}