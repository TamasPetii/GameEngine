#version 460 core

layout(location = 0) in vec3 vs_in_pos;

struct Transform
{
	mat4 model;
	mat4 modelIT;
};

layout(std430, binding = 0) buffer u_indexData
{   
    uint indexData[]; //(TransformID)
};

layout(std430, binding = 0) buffer u_transformData
{   
    Transform transforms[];
};

void main()
{
	gl_Position = transforms[u_indexData[gl_InstanceID]].model * vec4(vs_in_pos, 1);
}