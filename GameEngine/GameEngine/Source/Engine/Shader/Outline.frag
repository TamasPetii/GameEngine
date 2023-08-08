#version 460 core

//Output Data
out vec4 out_color;

uniform vec3 u_OutlineColor;

void main()
{	
	out_color = vec4(u_OutlineColor, 1);
}
