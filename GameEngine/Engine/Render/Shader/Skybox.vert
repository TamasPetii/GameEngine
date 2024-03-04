#version 460 core

layout(location = 0) in vec3 vs_in_pos;
layout(location = 0) out vec3 vs_out_pos;

layout(std140, binding = 0) uniform u_cameraData
{
    mat4 view;
    mat4 viewInv;
    mat4 proj;
    mat4 projInv;
    mat4 viewProj;
    mat4 viewProjInv;
    vec4 eye;
};

uniform mat4 model;

void main()
{
	gl_Position = (viewProj * model * vec4( vs_in_pos, 1 )).xyww;
	vs_out_pos = vs_in_pos;
}