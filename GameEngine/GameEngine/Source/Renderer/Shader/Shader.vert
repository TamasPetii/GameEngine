#version 460 core

//Input Data
layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec3 vert_tangent;
layout(location = 3) in vec3 vert_bitangent;
layout(location = 4) in vec2 vert_texture;
layout(location = 5) in mat4 vert_transform;

//Output Data
out vec3 geom_position;
out vec3 geom_normal;
out vec3 geom_tangent;
out vec3 geom_bitangent;
out vec2 geom_texture;
out vec4 geom_position_shadow;

//Uniforms
uniform int u_IsInstanced;
uniform mat4 u_VP = mat4(1);
uniform mat4 u_M = mat4(1);
uniform mat4 u_MIT = mat4(1);
uniform mat4 u_ShadowVP = mat4(1);

void main(){
	
	vec4 position;
	vec4 normal;
	vec4 tangent;
	vec4 bitangent;

	if(u_IsInstanced != 0)
	{
		mat4 vert_transformIT = transpose(inverse(vert_transform));
		position = vert_transform * vec4(vert_position, 1);
		normal = vert_transformIT * vec4(vert_normal, 1);
		tangent = vert_transformIT * vec4(vert_tangent, 1);
		bitangent = vert_transformIT * vec4(vert_bitangent, 1);
	}
	else
	{
		position = u_M * vec4(vert_position, 1);
		normal = u_MIT * vec4(vert_normal, 1);
		tangent = u_MIT * vec4(vert_tangent, 1);
		bitangent = u_MIT * vec4(vert_bitangent, 1);
	}


	gl_Position = u_VP * position;
	geom_position = position.xyz;
	geom_normal = normalize(normal.xyz);
	geom_tangent = normalize(tangent.xyz);
	geom_bitangent = normalize(bitangent.xyz);
	geom_texture = vec2(vert_texture.x, vert_texture.y);
	geom_position_shadow = u_ShadowVP * position;
}