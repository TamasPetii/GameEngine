#version 460 core

//Input Data
layout(location = 0) in vec3 vert_position;

//Output Data
out vec3 frag_position;

//Uniforms
uniform mat4 u_VP = mat4(1);
uniform mat4 u_M = mat4(1);

void main(){
	vec4 position = u_M * vec4(vert_position, 1);

	gl_Position = u_VP * position;
	frag_position = position.xyz;
}