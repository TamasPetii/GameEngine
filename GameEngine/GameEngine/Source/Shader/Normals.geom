#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

//Input data
in vec3 geom_normal[];

//Uniform data
uniform mat4 u_VP;

const float magintude = 0.05;

void GenerateLine(int index)
{
	gl_Position = u_VP * gl_in[index].gl_Position;
	EmitVertex();

	gl_Position = u_VP * (gl_in[index].gl_Position + vec4(geom_normal[index], 0) * magintude);
	EmitVertex();

	EndPrimitive();
}

void main()
{
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}