#version 460 core
#extension GL_ARB_bindless_texture : require

layout(location = 0) in flat uint fs_in_id;
layout(location = 0) out vec4     fs_out_col;

struct SpotLight
{
	vec4 color;
	vec4 position;
	vec4 direction;
	mat4 viewProj;
	vec2 angles;
	uvec2 shadowTexture;
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

layout(std430, binding = 2) buffer u_spotLightData
{
	SpotLight spotLightData[];
};

uniform sampler2D colorTexture;
uniform sampler2D additionalTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform float textureWidth;
uniform float textureHeight;

void main()
{
	vec2  fs_in_tex = vec2(gl_FragCoord.x / textureWidth, gl_FragCoord.y / textureHeight);
	float depth     = texture(depthTexture, fs_in_tex).x;
	vec3  depth_ndc = vec3(fs_in_tex.x, fs_in_tex.y, depth) * 2 - vec3(1);
	vec4  depth_pos = viewProjInv * vec4(depth_ndc, 1);
		
	vec3 position   = depth_pos.xyz / depth_pos.w;
	vec3 normal     = texture(normalTexture, fs_in_tex).xyz;
	vec3 additional = texture(additionalTexture, fs_in_tex).xyz;
	vec3 color      = texture(colorTexture, fs_in_tex).xyz;

	vec3 light_vec  = spotLightData[fs_in_id].position.xyz - position;
	float intensity = 1.0;
	//float intensity = 1.0 / dot(light_vec, light_vec);
	vec3 to_light   = normalize(light_vec);
	vec3 to_eye     = normalize(eye.xyz - position);
	float theta = dot(normalize(-light_vec), normalize(spotLightData[fs_in_id].direction.xyz));

	vec3 diffuse = vec3(0);
	vec3 specular = vec3(0);
	if(theta > spotLightData[fs_in_id].angles.y)
	{
		//Diffuse
		diffuse += intensity * spotLightData[fs_in_id].color.xyz * spotLightData[fs_in_id].color.w * color * clamp(dot(normal, to_light), 0, 1);
	
		//Specular
		vec3 reflection = normalize(reflect(-to_light, normal));
		specular += additional.x * intensity * spotLightData[fs_in_id].color.xyz * spotLightData[fs_in_id].color.w * color * pow(clamp(dot(to_eye, reflection), 0, 1), additional.y * 256);
	}

	fs_out_col = vec4(diffuse + specular, 1);
	//fs_out_col = vec4(color, 1);
}