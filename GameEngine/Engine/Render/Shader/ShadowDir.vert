#version 460 core

layout(location = 0) in vec3 vs_in_pos;

struct Transform
{
	mat4 model;
	mat4 modelIT;
};

layout(std430, binding = 0) buffer u_transformData
{   
    Transform transforms[];
};

uniform uint u_transformIndex;

void main()
{
	gl_Position = transforms[u_transformIndex].model * vec4(vs_in_pos, 1);
}