#version 460 core

//Input Data
layout(location = 0) in vec3 vert_position;

//Output Data
out vec3 frag_position;

uniform mat4 u_VP;
uniform mat4 u_M;

void main()
{
	frag_position = vert_position;
	gl_Position = (u_VP * u_M * vec4(vert_position, 1)).xyww;
}