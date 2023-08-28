#version 460 core

//Output Data
in vec3 frag_position;
in vec2 frag_texture;

//Output Data
out vec4 out_color;

//Uniforms
uniform sampler2D uSkysphereTexture;

void main()
{
	out_color = texture(uSkysphereTexture, frag_texture);
}