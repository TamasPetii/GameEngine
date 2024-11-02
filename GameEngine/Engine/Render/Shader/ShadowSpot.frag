#version 460 core

struct SpotLight
{
	vec4 color; //3 float color + 1 float strength
	vec4 position; //3 float position + 1 float use shadow
	vec4 direction; //3 float direction + 1 float farplane
	mat4 viewProj;
	vec4 angles;
	uvec2 shadowTexture;
	vec2 filler;
};

layout(std430, binding = 0) buffer u_spotLightData
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