#version 460 core

//Output Data
in vec2 frag_texture;

//Output Data
layout(location = 0) out vec4 out_color;
layout(location = 1) out unsigned int out_id;

//Uniforms
uniform sampler2D uSkyTexture;

void main()
{
	out_id = 0;
	out_color = texture(uSkyTexture, frag_texture);
}