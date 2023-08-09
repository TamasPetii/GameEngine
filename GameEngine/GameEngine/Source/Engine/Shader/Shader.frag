#version 460 core

//Input Data
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture;

//Output Data
out vec4 out_color;

//Uniforms
uniform sampler2D u_Texture;
uniform vec3 u_Color;
uniform int u_UseTexture;

void main()
{	
	if(u_UseTexture == 1)
	{	
		out_color = texture(u_Texture, frag_texture);
	}
	else
	{
		out_color = vec4(u_Color, 1);
	}
}