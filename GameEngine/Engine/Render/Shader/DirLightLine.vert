#version 460 core

layout(location = 0) in vec3 vs_in_pos;

struct Dirlight
{
	vec4 position;
    vec4 direction;
};

layout(std430, binding = 0) buffer u_cameraData
{
    mat4 view;
    mat4 viewInv;
    mat4 proj;
    mat4 projInv;
    mat4 viewProj;
    mat4 viewProjInv;
    vec4 eye;
};

layout(std430, binding = 1) buffer u_dirlightData
{
	Dirlight dirlightData[];
};

void main()
{
    vec4 position;

    if(gl_VertexID == 0)
        position = vec4(dirlightData[gl_InstanceID].position.xyz, 1);
    else
        position = vec4(dirlightData[gl_InstanceID].position.xyz + 3 * dirlightData[gl_InstanceID].direction.xyz, 1);
        
    gl_Position = viewProj * position;
} 