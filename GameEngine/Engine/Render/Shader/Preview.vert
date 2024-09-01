#version 460 core

layout(location = 0) in vec3 vs_in_pos;
layout(location = 1) in vec3 vs_in_norm;
layout(location = 2) in vec3 vs_in_tang;
layout(location = 3) in vec2 vs_in_tex;
layout(location = 4) in uint vs_in_id;

layout(location = 0) out vec3 vs_out_pos;
layout(location = 1) out vec3 vs_out_norm;
layout(location = 2) out vec2 vs_out_tex;
layout(location = 3) out flat uint vs_out_id; //Material

uniform mat4 u_viewProj;
uniform uint u_renderMode;

void main()
{
    vec4 position = vec4(vs_in_pos, 1.0);
    vec3 normal = normalize(vs_in_norm.xyz);

	gl_Position = u_viewProj * position;
    vs_out_pos  = position.xyz;
    vs_out_norm = normal.xyz;
    vs_out_tex  = vs_in_tex;
    vs_out_id = u_renderMode == 0 ? 0 : vs_in_id;
} 