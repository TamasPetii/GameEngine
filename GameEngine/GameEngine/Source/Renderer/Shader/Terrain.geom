#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

//Input Data
in vec3 geom_position[];
in vec2 geom_texture[];

//Output Data
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;

//Uniform 
uniform mat4 u_VP = mat4(1);
uniform mat4 u_M = mat4(1);
uniform mat4 u_MIT = mat4(1);

void main()
{
	vec3 edge1 = geom_position[1] - geom_position[0];
	vec3 edge2 = geom_position[2] - geom_position[0];
	vec3 normal = normalize(cross(edge1, edge2));

	for(int i = 0; i < 3; i++)
	{	
		gl_Position = gl_in[i].gl_Position;
		frag_position = geom_position[i];
		frag_normal = normal;
		frag_texture = geom_texture[i];
		EmitVertex();
	}

	EndPrimitive();
}