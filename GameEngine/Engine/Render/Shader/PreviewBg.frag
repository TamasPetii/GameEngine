#version 460 core
#extension GL_ARB_bindless_texture : require

layout(location = 0) in vec2 fs_in_tex;
layout(location = 0) out vec3 fs_out_col;

/*
struct Dirlight
{
	vec4 color;
	vec4 direction;
	vec4 farPlane;
	mat4 viewProj[4];
	uvec2 shadowTexture;
	uvec2 filler;
};

layout(std430, binding = 1) buffer u_dirlightData
{
	Dirlight dirlightData[];
};
uniform uint layer;
*/

uniform sampler2D u_bgTexture;

void main()
{
  /*
    if(layer < 4)
    {
        fs_out_col = vec3(texture(sampler2DArray(dirlightData[0].shadowTexture), vec3(fs_in_tex, layer)).x);
        return;
	}
  */

    fs_out_col = texture(u_bgTexture, fs_in_tex).xyz;
}