#version 460 core

//Input Data
layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 vert_texture;

//Output Data
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;
out vec4 frag_position_shadow;

//Uniforms
uniform mat4 u_VP = mat4(1);
uniform mat4 u_M = mat4(1);
uniform mat4 u_MIT = mat4(1);
uniform mat4 u_ShadowVP = mat4(1);

void main(){
	vec4 position = u_M * vec4(vert_position, 1);
	vec4 normal = u_MIT * vec4(vert_normal, 1);

	gl_Position = u_VP * position;
	frag_position = position.xyz;
	frag_normal = normalize(normal.xyz);
	frag_texture = vec2(vert_texture.x, 1 - vert_texture.y);
	frag_position_shadow = u_ShadowVP * position;
}