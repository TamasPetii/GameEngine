#version 460 core

layout(std430, binding = 1) buffer u_positions
{
	vec4 positions[];
};

void main()
{
	gl_Position = positions[gl_VertexID];
}