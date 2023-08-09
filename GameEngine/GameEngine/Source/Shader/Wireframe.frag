#version 460 core

//Output Data
out vec4 out_color;

//Uniform Data
uniform vec3 u_Color;

void main()
{	
	out_color = vec4(u_Color,1);
}
