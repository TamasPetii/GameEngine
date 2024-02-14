#version 460 core
layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

layout(std140, binding = 0) uniform u_cameraData
{
    mat4 view;
    mat4 viewInv;
    mat4 proj;
    mat4 projInv;
    mat4 viewProj;
    mat4 viewProjInv;
    vec4 eye;
};

layout(location = 0) out vec2 gs_out_tex;
layout(location = 1) out flat uint gs_out_id;

void main()
{	
	vec3 to_eye = eye.xyz - gl_in[0].gl_Position.xyz;
	vec3 to_up  = vec3(0,1,0);
	vec3 to_right = normalize(cross(to_eye, to_up));

	//Bottom Left
	gl_Position = viewProj * vec4(gl_in[0].gl_Position.xyz - to_up - to_right, 1);
	gs_out_tex = vec2(0, 0);
	gs_out_id = uint(gl_in[0].gl_Position.w);
	EmitVertex();

	//Top Left
	gl_Position = viewProj * vec4(gl_in[0].gl_Position.xyz + to_up - to_right, 1);
	gs_out_tex = vec2(0, 1);
	gs_out_id = uint(gl_in[0].gl_Position.w);
	EmitVertex();

	//Bottom Right
	gl_Position = viewProj * vec4(gl_in[0].gl_Position.xyz - to_up + to_right, 1);
	gs_out_tex = vec2(1, 0);
	gs_out_id = uint(gl_in[0].gl_Position.w);
	EmitVertex();

	//Top Right
	gl_Position = viewProj * vec4(gl_in[0].gl_Position.xyz + to_up + to_right, 1);
	gs_out_tex = vec2(1, 1);
	gs_out_id = uint(gl_in[0].gl_Position.w);
	EmitVertex();

	EndPrimitive();

}