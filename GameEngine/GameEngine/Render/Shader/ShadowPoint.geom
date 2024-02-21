#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;
layout(location = 0) out vec4 gs_out_pos;

struct PointLight
{
	vec4 color;
	vec4 position;
	mat4 viewProj[6];
	vec2 farPlane;
	uvec2 shadowTexture;
};

layout(std430, binding = 1) buffer u_pointLightData
{
	PointLight pointLightData[];
};

uniform uint u_lightIndex;

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face;
        for(int i = 0; i < 3; ++i)
        {
            gs_out_pos = gl_in[i].gl_Position;
            gl_Position = pointLightData[u_lightIndex].viewProj[face] * gs_out_pos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  