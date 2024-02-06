#version 460 core

layout(location = 0) in vec3 vs_in_position;
layout(location = 1) in vec3 vs_in_normal;
layout(location = 2) in vec2 vs_in_texcoord;

layout(location = 0) out vec3 vs_out_position;
layout(location = 1) out vec3 vs_out_normal;
layout(location = 2) out vec2 vs_out_texcoord;
layout(location = 3) out flat uint vs_out_id;

struct Transform
{
	mat4 model;
	mat4 modelIT;
};

layout(std140, binding = 0) uniform u_cameraData
{
    mat4 view;
    mat4 viewInv;
    mat4 proj;
    mat4 projInv;
    mat4 viewProj;
    mat4 viewProjInv;
    vec4 position;
};

layout(std430, binding = 1) buffer u_transformData
{   
    Transform transforms[];
};

void main()
{
    vec4 position = transforms[gl_InstanceID].model * vec4(vs_in_position, 1.0);
    vec4 normal = transforms[gl_InstanceID].modelIT * vec4(vs_in_normal, 0.0);
    gl_Position = viewProj * position;

    vs_out_position = position.xyz;
    vs_out_normal   = vs_in_normal.xyz;
    vs_out_texcoord = vs_in_texcoord;
    vs_out_id = uint(gl_InstanceID);
} 