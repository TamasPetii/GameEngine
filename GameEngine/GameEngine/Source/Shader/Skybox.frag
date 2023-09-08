#version 460 core

//Input Data
in vec3 frag_position;

//Output Data
layout(location = 0) out vec4 out_color;

//Uniforms
uniform samplerCube uSkyTexture;

void main()
{
	out_color = texture(uSkyTexture, frag_position);
}