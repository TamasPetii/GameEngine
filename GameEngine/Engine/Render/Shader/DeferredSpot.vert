#version 460 core

layout(location = 0) in vec3 vs_in_pos;
layout(location = 0) out flat uint vs_out_id;

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

layout(std430, binding = 1) buffer u_proxyConeTransform
{
	mat4 proxyConeTransform[];
};

void main()
{
	gl_Position = viewProj * proxyConeTransform[gl_InstanceID] * vec4(vs_in_pos, 1);
	vs_out_id = uint(gl_InstanceID);
}