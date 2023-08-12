#version 460 core

out vec4 out_color;

void main()
{         
	out_color = vec4(vec3(gl_FragCoord.z), 1);
}  