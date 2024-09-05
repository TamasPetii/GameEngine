#version 460 core

layout(location = 0) in vec3 vs_in_pos;

struct Transform
{
	mat4 model;
	mat4 modelIT;
};

layout(std430, binding = 1) buffer u_transformData
{   
    Transform transformData[];
};

layout(std430, binding = 2) buffer u_indexData
{   
    uint indexData[]; //(TransformID)
};

uniform uint u_transformIndex;
uniform uint u_renderMode;

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

	gl_Position = position;
}