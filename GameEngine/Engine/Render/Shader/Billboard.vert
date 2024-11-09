#version 460 core

layout(location = 0) out flat uint vs_out_id;

layout(std430, binding = 1) buffer u_positions
{
	vec4 positions[];
};

void main()
{
	gl_Position = positions[gl_VertexID];
	vs_out_id = uint(gl_VertexID);
}