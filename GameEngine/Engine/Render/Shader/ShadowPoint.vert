#version 460 core

layout (location = 0) in vec3 vs_in_pos;

struct Transform
{
	mat4 model;
	mat4 modelIT;
};

struct VertexBoneData
{
	vec4 boneWeights;
	ivec4 boneIndices;
};

layout(std430, binding = 1) buffer u_transformData
{   
    Transform transformData[];
};

layout(std430, binding = 2) buffer u_indexData
{   
	uint indexData[]; //(TransformID)
};

layout(std430, binding = 3) buffer u_boneTransform
{
	mat4 boneTransforms[];
};

layout(std430, binding = 4) buffer u_vertexBones
{
	VertexBoneData vertexBones[];
};

uniform uint u_renderMode;
uniform uint u_transformIndex;

void main()
{
    vec4 position;

	//Normal Shape Rendering | Model Rendering
	if(u_renderMode == 0 || u_renderMode == 2)
	{
		position = transformData[u_transformIndex].model * vec4(vs_in_pos, 1);
	}
	//Instanced Shape Rendering | Instanced Model Rendering
    else if(u_renderMode == 1 || u_renderMode == 3)
	{	
		position = transformData[indexData[gl_InstanceID]].model * vec4(vs_in_pos, 1);
	}
	//Animation
	else if(u_renderMode == 4)
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
	}

	gl_Position = position;
}  