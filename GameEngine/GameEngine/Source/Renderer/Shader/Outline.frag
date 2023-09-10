#version 460 core

//Output Data
layout(location = 0) out vec4 out_color;

uniform vec3 u_OutlineColor;

void main()
{	
	out_color = vec4(u_OutlineColor, 1);
}
