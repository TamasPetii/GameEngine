#version 460 core

//Input Data
layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_normal;

//Output Data
out vec3 geom_normal;

//Uniforms
uniform mat4 u_M = mat4(1);
uniform mat4 u_MIT = mat4(1);

void main(){
	vec4 normal = u_MIT * vec4(vert_normal, 1);
	vec4 position = u_M * vec4(vert_position, 1);
	
	geom_normal = normal.xyz;
	gl_Position = position;
}