#version 460 core
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

layout(std430, binding = 0) buffer u_cameraData
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
uniform sampler2D positionTexture;

float LinearizeDepth(float depth, float near, float far) 
{
    return ((2.0 * near * far) / (far + near - depth * (far - near))) / far; // 0-1
}

void main()
{
	/*
	float depth     = texture(depthTexture, fs_in_tex).x;
	vec3  depth_ndc = vec3(fs_in_tex.x, fs_in_tex.y, depth) * 2 - vec3(1);
	vec4  depth_pos = viewProjInv * vec4(depth_ndc, 1);
	vec3 position   = depth_pos.xyz / depth_pos.w;
	*/

	vec3 position   = texture(positionTexture, fs_in_tex).xyz;
	
	vec3 color      = texture(colorTexture, fs_in_tex).xyz;
	vec3 additional = texture(additionalTexture, fs_in_tex).xyz;
	vec3 normal     = normalize(texture(normalTexture, fs_in_tex).xyz);

	vec3 to_light = normalize(-dirlightData[fs_in_id].direction.xyz);
	vec3 to_eye   = normalize(eye.xyz - position);

	//Diffuse
	float diffureAttenuation = clamp(dot(normal, to_light), 0, 1);
	vec3 diffuse = dirlightData[fs_in_id].color.xyz * dirlightData[fs_in_id].color.w * color * diffureAttenuation;

	float shadow = 0.0;
	vec3 specular = vec3(0);

	if(diffureAttenuation > 0.00001)
	{
		//Specular
		vec3 reflection = normalize(reflect(-to_light, normal));
		specular = additional.x * dirlightData[fs_in_id].color.xyz * dirlightData[fs_in_id].color.w * color * pow(clamp(dot(to_eye, reflection), 0, 1), additional.y * 256);

		if(dirlightData[fs_in_id].direction.w != 0 && additional.z != 0 && dirlightData[fs_in_id].shadowTexture != uvec2(0))
		{
			float frag_depth_view = (view * vec4(position, 1)).z * -1;
			int cascade_index = frag_depth_view < dirlightData[fs_in_id].farPlane[0] ? 0 : frag_depth_view < dirlightData[fs_in_id].farPlane[1] ? 1 : frag_depth_view < dirlightData[fs_in_id].farPlane[2] ? 2 : 3;

			float bias = max(0.05 * (1.0 - dot(normal, -1 * normalize(dirlightData[fs_in_id].direction.xyz))), 0.005)  / (dirlightData[fs_in_id].farPlane[cascade_index] * 0.5); 	
			vec4 position_shadow = dirlightData[fs_in_id].viewProj[cascade_index] * vec4(position, 1);
			vec3 lightCoords = 0.5 * (position_shadow.xyz / position_shadow.w) + 0.5;
		
			float pcfDepth = texture(sampler2DArray(dirlightData[fs_in_id].shadowTexture), vec3(lightCoords.xy, cascade_index)).x; 
			float fragDepth = (lightCoords.z - bias);
			shadow += fragDepth > pcfDepth ? 1.0 : 0.0;
		}
	}

	fs_out_col = vec4((diffuse + specular) * (1.0 - shadow), 1.0);
}