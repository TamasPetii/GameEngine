#version 460 core
#extension GL_ARB_bindless_texture : require

layout(location = 0) in flat uint fs_in_id;
layout(location = 0) out vec4     fs_out_col;

struct PointLight
{
	vec4 color; //3 float color + 1 float strength
	vec4 position; //3 float position + 1 float use shadow
	mat4 viewProj[6];
	vec2 farPlane; //1 float farplane + 1 float filler
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

layout(std430, binding = 2) buffer u_pointLightData
{
	PointLight pointLightData[];
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

	if(abs(distance(position, pointLightData[fs_in_id].position.xyz)) > pointLightData[fs_in_id].farPlane.x)
		discard;

	vec3 normal     = texture(normalTexture, fs_in_tex).xyz;
	vec3 additional = texture(additionalTexture, fs_in_tex).xyz;
	vec3 color      = texture(colorTexture, fs_in_tex).xyz;

	vec3 light_vec  = pointLightData[fs_in_id].position.xyz - position;
	float intensity = 1.0;
	//float intensity = 1.0 / dot(light_vec, light_vec);
	vec3 to_light   = normalize(light_vec);
	vec3 to_eye     = normalize(eye.xyz - position);

	//Diffuse
	vec3 diffuse = intensity * pointLightData[fs_in_id].color.xyz * pointLightData[fs_in_id].color.w * color * clamp(dot(normal, to_light), 0, 1);
	
	//Specular
	vec3 reflection = normalize(reflect(-to_light, normal));
	vec3 specular = additional.x * intensity * pointLightData[fs_in_id].color.xyz * pointLightData[fs_in_id].color.w * color * pow(clamp(dot(to_eye, reflection), 0, 1), additional.y * 256);

	float shadow = 0.0;
	if(pointLightData[fs_in_id].position.w != 0 && additional.z != 0 && pointLightData[fs_in_id].shadowTexture != uvec2(0))
	{
		vec3 fragToLight = position - pointLightData[fs_in_id].position.xyz;
		float closestDepth = texture(samplerCube(pointLightData[fs_in_id].shadowTexture), fragToLight).r * pointLightData[fs_in_id].farPlane.x;
		float currentDepth = length(fragToLight);  
		float bias = 0.15;
		shadow = currentDepth > closestDepth + bias ? 1.0 : 0.0; 	
	}

	fs_out_col = vec4((diffuse + specular) * (1 - shadow), 1);
}