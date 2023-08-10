#version 460 core

out vec4 out_color;
uniform vec3 u_Color;

void main()
{         
	out_color = vec4(vec3(u_Color), 1);
	out_color = vec4(vec3(gl_FragCoord.z), 1);
}  