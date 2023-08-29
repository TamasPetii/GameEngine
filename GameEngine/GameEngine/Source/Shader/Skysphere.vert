#version 460 core

//Input Data
layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 vert_texture;

//Output Data
out vec3 frag_position;
out vec2 frag_texture;

uniform mat4 u_VP;
uniform mat4 u_M;

void main()
{
	frag_position = vert_position;
	frag_texture = vert_texture;
	gl_Position = (u_VP * u_M * vec4(vert_position, 1)).xyww;
}