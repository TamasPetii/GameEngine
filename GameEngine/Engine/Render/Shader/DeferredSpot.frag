#version 460 core
#extension GL_ARB_bindless_texture : require

layout(location = 0) in flat uint fs_in_id;
layout(location = 0) out vec4     fs_out_col;

struct SpotLight
{
	vec4 color; //3 float color + 1 float strength
	vec4 position; //3 float position + 1 float use shadow
	vec4 direction; //3 float direction + 1 float farplane
	mat4 viewProj;
	vec4 angles;
	uvec2 shadowTexture;
	vec2 filler;
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

layout(std430, binding = 2) buffer u_spotLightData
{
	SpotLight spotLightData[];
};

uniform sampler2D colorTexture;
uniform sampler2D additionalTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform sampler2D positionTexture;

uniform float textureWidth;
uniform float textureHeight;
uniform float bias = 0.005;

float LinearizeDepth(float depth, float near, float far) 
{
    return ((2.0 * near * far) / (far + near - depth * (far - near))) / far; // 0-1
}

void main()
{
	vec2  fs_in_tex = vec2(gl_FragCoord.x / textureWidth, gl_FragCoord.y / textureHeight);

	/*
	float depth     = texture(depthTexture, fs_in_tex).x;
	vec3  depth_ndc = vec3(fs_in_tex.x, fs_in_tex.y, depth) * 2 - vec3(1);
	vec4  depth_pos = viewProjInv * vec4(depth_ndc, 1);	
	vec3 position   = depth_pos.xyz / depth_pos.w;
	*/

	vec3 position = texture(positionTexture, fs_in_tex).xyz;

	//Check if fragment position is inside the proxy cone
	vec3 fromSpotToFrag = position - spotLightData[fs_in_id].position.xyz;
	vec3 fromSpotToFragNorm = normalize(fromSpotToFrag);
	vec3 toSpotDirNorm = normalize(spotLightData[fs_in_id].direction.xyz);
	float alpha = dot(toSpotDirNorm, fromSpotToFragNorm); // Angle tells if we are inside the cone
	float delta = dot(toSpotDirNorm, fromSpotToFrag); //The projected length to check if inside the far plane
	if(alpha < spotLightData[fs_in_id].angles.w || delta > spotLightData[fs_in_id].direction.w)
		discard;

	vec3 normal     = normalize(texture(normalTexture, fs_in_tex).xyz);
	vec3 additional = texture(additionalTexture, fs_in_tex).xyz;
	vec3 color      = texture(colorTexture, fs_in_tex).xyz;

	vec3 light_vec  = spotLightData[fs_in_id].position.xyz - position;
	vec3 to_light   = normalize(light_vec);
	vec3 to_eye     = normalize(eye.xyz - position);

	float intensity = 1;
	
	float cosTheta = dot(normalize(spotLightData[fs_in_id].direction.xyz), -to_light);
	if(cosTheta < spotLightData[fs_in_id].angles.z)
	{
		float theta = degrees(acos(cosTheta));
		intensity = clamp((spotLightData[fs_in_id].angles.y - theta) / (spotLightData[fs_in_id].angles.y - spotLightData[fs_in_id].angles.x), 0.0, 1.0);
	}

	//Diffuse
	vec3 diffuse = intensity * spotLightData[fs_in_id].color.xyz * spotLightData[fs_in_id].color.w * color * clamp(dot(normal, to_light), 0, 1);
	
	//Specular
	vec3 reflection = normalize(reflect(-to_light, normal));
	vec3 specular = additional.x * intensity * spotLightData[fs_in_id].color.xyz * spotLightData[fs_in_id].color.w * color * pow(clamp(dot(to_eye, reflection), 0, 1), additional.y * 256);

	float shadow = 0.0;
	if(spotLightData[fs_in_id].position.w != 0 && additional.z != 0 && spotLightData[fs_in_id].shadowTexture != uvec2(0))
	{
		vec4 shadowPos = spotLightData[fs_in_id].viewProj * vec4(position, 1);
		shadowPos.xyz /= shadowPos.w;
		vec2 shadowTex = shadowPos.xy * 0.5 + vec2(0.5);
		float currentDepth = LinearizeDepth(shadowPos.z, 0.01, spotLightData[fs_in_id].direction.w);
		float storedDepth = texture(sampler2D(spotLightData[fs_in_id].shadowTexture), shadowTex).x;

		float bia = max(0.005 * (1.0 - dot(normal, -1 * normalize(to_light))), 0.0005);
		shadow = currentDepth > storedDepth + bia ? 1.0 : 0.0;
	}

	fs_out_col = vec4((diffuse + specular) * (1 - shadow), 1);
}