#version 460 core
layout (location = 0) in vec3 vs_in_pos;

struct Transform
{
	mat4 model;
	mat4 modelIT;
};

struct SpotLight
{
	vec4 color;
	vec4 position;
	vec4 direction;
	mat4 viewProj;
	vec2 angles;
	uvec2 shadowTexture;
};

layout(std430, binding = 0) buffer u_transformData
{   
    Transform transformData[];
};

layout(std430, binding = 1) buffer u_spotLightData
{
	SpotLight spotLightData[];
};

uniform uint u_lightIndex;
uniform uint u_transformIndex;

void main()
{
    gl_Position = spotLightData[u_lightIndex].viewProj * transformData[u_transformIndex].model * vec4(vs_in_pos, 1.0);
}  