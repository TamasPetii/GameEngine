#version 460 core
   
layout(triangles, invocations = 4) in;
layout(triangle_strip, max_vertices = 3) out;

struct Dirlight
{
	vec4 color;
	vec4 direction;
	vec4 farPlane;
	mat4 viewProj[4];
	uvec2 shadowTexture;
};

layout(std430, binding = 1) buffer u_dirlightData
{
	Dirlight dirlightData[];
};

uniform uint u_lightIndex;
    
void main()
{          
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = dirlightData[u_lightIndex].viewProj[gl_InvocationID] * gl_in[i].gl_Position;
        gl_Layer = gl_InvocationID;
        EmitVertex();
    }
    EndPrimitive();
}  