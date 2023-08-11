#version 460 core

//Input Data
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture;

//Output Data
out vec4 out_color;

void main()
{	
	out_color = vec4(normalize(frag_position) * frag_normal, 1);
}
