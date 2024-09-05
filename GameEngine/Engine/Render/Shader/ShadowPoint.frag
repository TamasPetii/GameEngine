#version 460 core

layout(location = 0) in vec4 fs_in_pos;

struct PointLight
{
	vec4 color;
	vec4 position;
	mat4 viewProj[6];
	vec2 farPlane;
	uvec2 shadowTexture;
};

layout(std430, binding = 0) buffer u_pointLightData
{
	PointLight pointLightData[];
};

uniform uint u_lightIndex;

void main()
{
    gl_FragDepth = length(fs_in_pos.xyz - pointLightData[u_lightIndex].position.xyz) / pointLightData[u_lightIndex].farPlane.x;
}  