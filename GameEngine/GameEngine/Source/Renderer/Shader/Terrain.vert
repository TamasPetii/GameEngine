#version 460 core

//Input Data
layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec3 vert_tangent;
layout(location = 3) in vec3 vert_bitangent;
layout(location = 4) in vec2 vert_texture;

//Output Data
out vec3 geom_position;
out vec2 geom_texture;

//Uniforms
uniform mat4 u_VP = mat4(1);
uniform mat4 u_M = mat4(1);
uniform mat4 u_MIT = mat4(1);

void main()
{	
	vec4 position = u_M * vec4(vert_position, 1);
	vec4 normal = u_MIT * vec4(vert_normal, 1);
	vec4 tangent = u_MIT * vec4(vert_tangent, 1);
	vec4 bitangent = u_MIT * vec4(vert_bitangent, 1);

	gl_Position = u_VP * position;
	geom_position = position.xyz;
	geom_texture = vec2(vert_texture.x, vert_texture.y);
}