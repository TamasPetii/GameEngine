#version 460
#extension GL_ARB_bindless_texture : require

layout(location = 0) in vec2 fs_in_tex;
layout(location = 1) in flat uint fs_in_id;
layout(location = 0) out vec4 fs_out_col;

struct Dirlight
{
	vec4 color;
	vec4 direction;
	vec4 farPlane;
	mat4 viewProj[4];
	uvec2 shadowTexture;
	uvec2 filler;
};

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

layout(std430, binding = 1) buffer u_dirlightData
{
	Dirlight dirlightData[];
};

uniform sampler2D colorTexture;
uniform sampler2D additionalTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;

uniform vec3 lightDir = vec3(-0.23,-0.5,-0.82);
uniform vec3 lightColor = vec3(1, 1, 1);

void main()
{
	float depth     = texture(depthTexture, fs_in_tex).x;
	vec3  depth_ndc = vec3(fs_in_tex.x, fs_in_tex.y, depth) * 2 - vec3(1);
	vec4  depth_pos = viewProjInv * vec4(depth_ndc, 1);
	
	vec3 position   = depth_pos.xyz / depth_pos.w;
	vec3 color      = texture(colorTexture, fs_in_tex).xyz;
	vec3 additional = texture(additionalTexture, fs_in_tex).xyz;
	vec3 normal     = normalize(texture(normalTexture, fs_in_tex).xyz);

	vec3 to_light = normalize(-dirlightData[fs_in_id].direction.xyz);
	vec3 to_eye   = normalize(eye.xyz - position);

	//Diffuse
	vec3 diffuse = dirlightData[fs_in_id].color.xyz * dirlightData[fs_in_id].color.w * color * clamp(dot(normal, to_light), 0, 1);

	//Specular
	vec3 reflection = normalize(reflect(-to_light, normal));
	vec3 specular = additional.x * dirlightData[fs_in_id].color.xyz * dirlightData[fs_in_id].color.w * color * pow(clamp(dot(to_eye, reflection), 0, 1), additional.y * 256);

	float shadow = 0.0;
	if(dirlightData[fs_in_id].direction.w != 0 && additional.z != 0 && dirlightData[fs_in_id].shadowTexture != uvec2(0))
	{
		float frag_depth_view = (view * vec4(position, 1)).z * -1;

		for(int i = 0; i < 4; i++)
		{
			if(frag_depth_view < dirlightData[fs_in_id].farPlane[i])
			{
				float bias = max(0.05 * (1.0 - dot(normal, -1 * normalize(dirlightData[fs_in_id].direction.xyz))), 0.005) / (dirlightData[fs_in_id].farPlane[i] * 0.5); 	
				vec4 position_shadow = dirlightData[fs_in_id].viewProj[i] * vec4(position, 1);
				vec3 lightCoords = 0.5 * (position_shadow.xyz / position_shadow.w) + 0.5;
				vec2 lightUV = lightCoords.xy;

				float d = texture(sampler2DArray(dirlightData[fs_in_id].shadowTexture), vec3(lightUV, i)).x;
				shadow += lightCoords.z > d + bias ? 1.0 : 0.0;   
				break;
			}
		}
	}

	fs_out_col = vec4((diffuse + specular) * (1 - shadow), 1);
}