#version 460 core

//Output Data
in vec2 frag_texture;

//Output Data
layout(location = 0) out vec4 out_color;

//Uniforms
uniform sampler2D uSkyTexture;

void main()
{
	out_color = texture(uSkyTexture, frag_texture);
}