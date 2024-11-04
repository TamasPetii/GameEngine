#version 460 core

layout(location = 0) in vec3 vs_in_pos;
layout(location = 1) in vec3 vs_in_norm;
layout(location = 2) in vec3 vs_in_tang;
layout(location = 3) in vec2 vs_in_tex;
layout(location = 4) in uint vs_in_id;

layout(location = 0) out vec3 vs_out_pos;
layout(location = 1) out vec3 vs_out_norm;
layout(location = 2) out vec2 vs_out_tex;
layout(location = 3) out flat uint vs_out_matID;

struct VertexBoneData
{
	vec4 boneWeights;
	ivec4 boneIndices;
};

struct Transform
{
	mat4 model;
	mat4 modelIT;
};

struct Water
{
    vec4 plane;
    vec2 scale;
    vec2 waveStrength;
    vec2 waveMove;
    uvec2 dudvTexture;
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

layout(std430, binding = 2) buffer u_waterData
{
    Water water[];
};

layout(std430, binding = 3) buffer u_indexData
{   
    uvec4 indexData[]; //(EntityID, TransformID, MaterialID)
};

layout(std430, binding = 5) buffer u_boneTransform
{
	mat4 boneTransforms[];
};

layout(std430, binding = 6) buffer u_vertexBones
{
	VertexBoneData vertexBones[];
};

uniform uint u_transformIndex;
uniform uint u_materialIndex;
uniform uint u_entityIndex;

uniform uint u_waterIndex;
uniform uint u_reflection;
uniform uint u_renderMode;
uniform mat4 u_skyboxModel;

void main()
{
    vec4 position;
    vec3 normal;

    //Normal Shape Rendering | Model Rendering
    if(u_renderMode == 0 || u_renderMode == 2)
    {
        position = transformData[u_transformIndex].model * vec4(vs_in_pos, 1.0);
        normal   = normalize(vec3(transformData[u_transformIndex].modelIT * vec4(vs_in_norm, 0.0)));
        vs_out_matID = u_renderMode == 0 ? u_materialIndex : vs_in_id;
    }
    //Instanced Shape Rendering | Instanced Model Rendering
    else if(u_renderMode == 1 || u_renderMode == 3)
    {
        position = transformData[indexData[gl_InstanceID].y].model * vec4(vs_in_pos, 1.0);
        normal   = normalize(vec3(transformData[indexData[gl_InstanceID].y].modelIT * vec4(vs_in_norm, 0.0)));
        vs_out_matID = u_renderMode == 1 ? indexData[gl_InstanceID].z : vs_in_id;
    }
    //Skybox
    else if(u_renderMode == 4)
    {
        gl_ClipDistance[0] = 1;
        gl_Position = (viewProj * u_skyboxModel * vec4(vs_in_pos, 1)).xyww;
        vs_out_pos = vs_in_pos;
        return;
    }
    //Animation
    else if(u_renderMode == 5)
    {
        vec4 totalPosition = vec4(0);

		for(int i = 0; i < 4; i++)
		{
			if(vertexBones[gl_VertexID].boneIndices[i] == -1)
			   continue;

			vec4 localPosition = boneTransforms[vertexBones[gl_VertexID].boneIndices[i]] * vec4(vs_in_pos, 1);
			totalPosition += localPosition * vertexBones[gl_VertexID].boneWeights[i];
		}

		position = transformData[u_transformIndex].model * totalPosition;
        normal   = normalize(vec3(transformData[u_transformIndex].modelIT * vec4(vs_in_norm, 0.0)));
        vs_out_matID = vs_in_id;
    }

    gl_ClipDistance[0] = dot(position, water[u_waterIndex].plane * (u_reflection == 0 ? vec4(1, 1, 1, -1) : vec4(1, -1, 1, 1)));      
    gl_Position = viewProj * position;

    vs_out_pos  = position.xyz;
    vs_out_norm = normal.xyz;
    vs_out_tex = vs_in_tex;
} 