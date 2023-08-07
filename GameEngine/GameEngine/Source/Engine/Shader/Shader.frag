#version 460 core

//Input Data
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture;

//Output Data
out vec4 out_color;

//Uniforms
uniform sampler2D u_Texture;

void main()
{	
	out_color = texture(u_Texture, frag_texture);
	//out_color = vec4(frag_position,1);
}