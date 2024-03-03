#version 460 core

layout(location = 0) in vec3 vs_in_pos;
layout(location = 1) in vec3 vs_in_norm;
layout(location = 2) in vec3 vs_in_tang;
layout(location = 3) in vec2 vs_in_tex;

layout(location = 0) out vec3 vs_out_pos;
layout(location = 1) out vec3 vs_out_norm;
layout(location = 2) out vec2 vs_out_tex;
layout(location = 3) out mat3 vs_out_tbn;

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
    vec4 eye;
};

layout(std430, binding = 1) buffer u_transformData
{   
    Transform transformData[];
};

uniform uint u_transformIndex;
uniform uint u_materialIndex;

void main()
{
    vec4 position = transformData[u_transformIndex].model * vec4(vs_in_pos, 1.0);
    vec3 normal   = normalize(vec3(transformData[u_transformIndex].modelIT * vec4(vs_in_norm, 0.0)));
    vec3 tangent  = normalize(vec3(transformData[u_transformIndex].modelIT * vec4(vs_in_tang, 0.0)));
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(normal, tangent);
    
    gl_Position = viewProj * position;

    vs_out_pos  = position.xyz;
    vs_out_norm = normal.xyz;
    vs_out_tex  = vs_in_tex;
    vs_out_tbn = mat3(tangent, bitangent, normal);
} 