#version 460 core

//Input Data
in vec3 frag_position;

//Output Data
out vec4 out_color;

//Uniforms
uniform samplerCube uSkyboxTexture;

void main()
{
	out_color = texture(uSkyboxTexture, frag_position);
}