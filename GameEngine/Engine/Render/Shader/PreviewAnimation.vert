#version 460 core

layout(location = 0) in vec3 vs_in_pos;
layout(location = 1) in vec3 vs_in_norm;
layout(location = 2) in vec3 vs_in_tang;
layout(location = 3) in vec2 vs_in_tex;
layout(location = 4) in uint vs_in_id;

layout(location = 0) out vec3 vs_out_pos;
layout(location = 1) out vec3 vs_out_norm;
layout(location = 2) out vec2 vs_out_tex;
layout(location = 3) out flat uint vs_out_id;

struct VertexBoneData
{
	vec4 boneWeights;
	ivec4 boneIndices;
};

layout(std430, binding = 0) buffer u_boneTransform
{
	mat4 boneTransforms[];
};

layout(std430, binding = 1) buffer u_vertexBones
{
	VertexBoneData vertexBones[];
};

uniform mat4 u_viewProj;

void main()
{
    vec4 totalPosition = vec4(0);

    for(int i = 0; i < 4; i++)
	{
		if(vertexBones[gl_VertexID].boneIndices[i] == -1)
		   continue;

		vec4 localPosition = boneTransforms[vertexBones[gl_VertexID].boneIndices[i]] * vec4(vs_in_pos, 1);
        totalPosition += localPosition * vertexBones[gl_VertexID].boneWeights[i];
	}

    vec4 position = totalPosition;
    vec3 normal   = normalize(vs_in_norm);
    
    gl_Position = u_viewProj * position;

    vs_out_pos  = position.xyz;
    vs_out_norm = normal.xyz;
    vs_out_tex  = vs_in_tex;
    vs_out_id = vs_in_id;
} 
