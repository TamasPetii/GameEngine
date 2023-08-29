#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

//Input Data
in vec3 geom_position[];
in vec3 geom_normal[];
in vec2 geom_texture[];
in vec4 geom_position_shadow[];

//Output Data
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;
out vec4 frag_position_shadow;
out mat3 frag_TBN;

//Uniform 
uniform mat4 u_VP = mat4(1);
uniform mat4 u_M = mat4(1);
uniform mat4 u_MIT = mat4(1);
uniform int normalMode = 2;

void main()
{
	vec3 normal;
	for(int i = 0; i < 3; i++)
	{	
		if(normalMode == 0)
		{
			normal = geom_normal[i];
		}
		else if(normalMode == 1)
		{
			vec3 edge1 = geom_position[1] - geom_position[0];
			vec3 edge2 = geom_position[2] - geom_position[0];
			normal = cross(edge1, edge2);
		}
		else if(normalMode == 2)
		{
			vec3 edge1 = geom_position[1] - geom_position[0];
			vec3 edge2 = geom_position[2] - geom_position[0];
			vec2 deultaUV1 = geom_texture[1] - geom_texture[0];
			vec2 deultaUV2 = geom_texture[2] - geom_texture[0];

			float det = 1.0 / (deultaUV1.x * deultaUV2.y - deultaUV2.x * deultaUV1.y);

			vec3 tangent;
			tangent.x = det * (edge1.x * deultaUV2.y - edge2.x * deultaUV1.y);
			tangent.y = det * (edge1.y * deultaUV2.y - edge2.y * deultaUV1.y);
			tangent.z = det * (edge1.z * deultaUV2.y - edge2.z * deultaUV1.y);

			vec3 bitangent;
			bitangent.x = det * (- edge1.x * deultaUV2.x + edge2.x * deultaUV1.x);
			bitangent.y = det * (- edge1.y * deultaUV2.x + edge2.y * deultaUV1.x);
			bitangent.z = det * (- edge1.z * deultaUV2.x + edge2.z * deultaUV1.x);
			
			vec3 T = normalize(tangent);
			vec3 B = normalize(bitangent);
			vec3 N = normalize(cross(edge1, edge2));
			frag_TBN = mat3(T, B, N);
		}

		gl_Position = gl_in[i].gl_Position;
		frag_normal = normalize(normal);
		frag_position = geom_position[i];
		frag_texture = geom_texture[i];
		frag_position_shadow = geom_position_shadow[i];
		EmitVertex();
	}
	EndPrimitive();
}