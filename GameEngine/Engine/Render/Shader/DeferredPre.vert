#version 460 core

layout(location = 0) in vec3 vs_in_pos;
layout(location = 1) in vec3 vs_in_norm;
layout(location = 2) in vec3 vs_in_tang;
layout(location = 3) in vec2 vs_in_tex;
layout(location = 4) in uint vs_in_id;

layout(location = 0) out vec3 vs_out_pos;
layout(location = 1) out vec3 vs_out_norm;
layout(location = 2) out vec2 vs_out_tex;
layout(location = 3) out flat uvec3 vs_out_id; //Entity-Material
layout(location = 4) out mat3 vs_out_tbn;

struct Transform
{
	mat4 model;
	mat4 modelIT;
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

layout(std430, binding = 1) buffer u_transformData
{   
    Transform transformData[];
};

layout(std430, binding = 2) buffer u_indexData
{   
    uvec4 indexData[]; //(EntityID, TransformID, MaterialID, ShapeModelID)
};

uniform uint u_shapeModelIndex;
uniform uint u_transformIndex;
uniform uint u_materialIndex;
uniform uint u_entityIndex;
uniform uint u_renderMode;

void main()
{
    vec4 position;
    vec3 normal;
    vec3 tangent;
    vec3 bitangent;

    //Normal Shape Rendering | Model Rendering
    if(u_renderMode == 0 || u_renderMode == 2)
    {
        position = transformData[u_transformIndex].model * vec4(vs_in_pos, 1.0);
        normal   = normalize(vec3(transformData[u_transformIndex].modelIT * vec4(vs_in_norm, 0.0)));
        tangent  = normalize(vec3(transformData[u_transformIndex].modelIT * vec4(vs_in_tang, 0.0)));
        tangent = normalize(tangent - dot(tangent, normal) * normal);
        bitangent = cross(normal, tangent);
        vs_out_id = u_renderMode == 0 ? uvec3(u_entityIndex, u_materialIndex, u_shapeModelIndex) : uvec3(u_entityIndex, vs_in_id, u_shapeModelIndex);
    }
    //Instanced Shape Rendering | Instanced Model Rendering
    else if(u_renderMode == 1 || u_renderMode == 3)
    {
        position = transformData[indexData[gl_InstanceID].y].model * vec4(vs_in_pos, 1.0);
        normal   = normalize(vec3(transformData[indexData[gl_InstanceID].y].modelIT * vec4(vs_in_norm, 0.0)));
        tangent  = normalize(vec3(transformData[indexData[gl_InstanceID].y].modelIT * vec4(vs_in_tang, 0.0)));
        tangent = normalize(tangent - dot(tangent, normal) * normal);
        bitangent = cross(normal, tangent);
        vs_out_id = u_renderMode == 1 ? uvec3(indexData[gl_InstanceID].x, indexData[gl_InstanceID].z, indexData[gl_InstanceID].w) : uvec3(indexData[gl_InstanceID].x, vs_in_id, indexData[gl_InstanceID].w);
    }
  
    gl_Position = viewProj * position;

    vs_out_pos  = position.xyz;
    vs_out_norm = normal.xyz;
    vs_out_tex  = vs_in_tex;
    vs_out_tbn = mat3(tangent, bitangent, normal);
} 