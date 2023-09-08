#version 460 core

//Input data
in vec3 frag_color;

//Output Data
out vec4 out_color;


void main()
{	
	out_color = vec4(frag_color, 1);
}
