#version 460 core

layout(location = 0) in vec3 vs_in_pos;

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

layout(std430, binding = 1) buffer u_transformData
{   
    mat4 transformData[];
};

layout(std430, binding = 2) buffer u_cameraViewProjInvData
{   
    mat4 cameraViewProjInvData[];
};

uniform uint u_renderMode;
uniform uint u_cullIndex;

void main()
{
    vec4 position = (u_renderMode == 0 ? transformData[gl_InstanceID] : cameraViewProjInvData[gl_InstanceID]) * vec4(vs_in_pos, 1.0);
    vec4 finalposition = (u_cullIndex != uint(gl_InstanceID) ? viewProj * position : vec4(2, 2, 2, 1));
    gl_Position = finalposition;
} 