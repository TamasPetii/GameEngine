#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 18) out;

//Input data
in vec3 geom_normal[];
in vec3 geom_tangent[];
in vec3 geom_bitangent[];

//Output data
out vec3 frag_color;

//Uniform data
uniform mat4 u_VP;

const float magintude = 0.05;

void GenerateLine(int index)
{
	//Normal
	gl_Position = u_VP * gl_in[index].gl_Position;
	frag_color = vec3(1,0,0);
	EmitVertex();

	gl_Position = u_VP * (gl_in[index].gl_Position + vec4(geom_normal[index], 0) * magintude);
	frag_color = vec3(1,0,0);
	EmitVertex();

	EndPrimitive();

	//Tangent
	gl_Position = u_VP * gl_in[index].gl_Position;
	frag_color = vec3(0,1,0);
	EmitVertex();

	gl_Position = u_VP * (gl_in[index].gl_Position + vec4(geom_tangent[index], 0) * magintude);
	frag_color = vec3(0,1,0);
	EmitVertex();

	EndPrimitive();

	//Bitangent
	gl_Position = u_VP * gl_in[index].gl_Position;
	frag_color = vec3(0,0,1);
	EmitVertex();

	gl_Position = u_VP * (gl_in[index].gl_Position + vec4(geom_bitangent[index], 0) * magintude);
	frag_color = vec3(0,0,1);
	EmitVertex();

	EndPrimitive();

}

void main()
{
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}