#version 460 core

struct SpotLight
{
	vec4 color;
	vec4 position;
	vec4 direction;
	mat4 viewProj;
	vec2 angles;
	uvec2 shadowTexture;
};

layout(std430, binding = 1) buffer u_spotLightData
{
	SpotLight spotLightData[];
};

uniform uint u_lightIndex;

float LinearizeDepth(float depth, float near, float far) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{   
	float near = 0.01;
	float far = spotLightData[u_lightIndex].direction.w;
	gl_FragDepth = LinearizeDepth(gl_FragCoord.z, near, far) / far;
}