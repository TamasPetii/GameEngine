#version 460 core
#define MAX_DIRECTION_LIGHT_COUNT 25
#define MAX_POINT_LIGHT_COUNT 25
#define MAX_SPOT_LIGHT_COUNT 25

struct DirectionLight
{
	vec3 direction;
	vec3 color;
	float diffuse;
	float specular;
	float volume;
};

struct PointLight
{
	vec3 position;
	vec3 color;
	float diffuse;
	float specular;
	float volume;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	vec3 color;
	float diffuse;
	float specular;
	float volume;
};

//Input Data
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture;
in vec4 frag_position_shadow;

//Output Data
out vec4 out_color;

//Uniforms
uniform sampler2D u_ShadowMap;
uniform vec3 u_CameraEye;
uniform vec3 u_Color;
uniform int u_UseTexture;
uniform sampler2D u_Texture;

uniform int u_DirectionLightCount = 0;
uniform DirectionLight u_DirectionLights[MAX_DIRECTION_LIGHT_COUNT];

uniform int u_PointLightCount = 0;
uniform PointLight u_PointLights[MAX_POINT_LIGHT_COUNT];

uniform int u_SpotLightCount = 0;
uniform SpotLight u_SpotLights[MAX_SPOT_LIGHT_COUNT];


vec3 CalculateDirectionLight(DirectionLight lightSource)
{
    vec3 toEye = normalize(u_CameraEye - frag_position);
	vec3 toLight = normalize(-lightSource.direction);
	vec3 normal = normalize(frag_normal);
	
	//Diffuse part
	float diffuseIntensity = clamp(dot(toLight, normal), 0, 1);
	vec3 diffuseColor = lightSource.color * lightSource.diffuse * lightSource.volume;
	vec3 diffuse = diffuseIntensity * diffuseColor;

	//Specular part
	vec3 specularReflection = normalize(reflect(-toLight, normal));
	float specularIntensity = pow(clamp(dot(toEye, specularReflection), 0, 1), 64);
	vec3 specularColor = lightSource.color * lightSource.specular * lightSource.volume;
	vec3 specular = specularIntensity * specularColor;

	return diffuse + specular;
}

vec3 CalculatePointLight(PointLight lightSource)
{
	vec3 toEye = normalize(u_CameraEye - frag_position);
	vec3 toLight = normalize(lightSource.position - frag_position);
	vec3 normal = normalize(frag_normal);
	float lightDistance = length(lightSource.position - frag_position);

	//Diffuse part
	float diffuseIntensity = clamp(dot(toLight, normal), 0, 1);
	vec3 diffuseColor = lightSource.color * lightSource.diffuse * lightSource.volume;
	vec3 diffuse = diffuseIntensity * diffuseColor * (1 / lightDistance);

	//Specular part
	vec3 specularReflection = normalize(reflect(-toLight, normal));
	float specularVolume = pow(clamp(dot(toEye, specularReflection), 0, 1), 64);
	vec3 specularColor = lightSource.color * lightSource.specular * lightSource.volume;
	vec3 specular = specularVolume * specularColor * (1 / lightDistance);

	return diffuse + specular;
}

vec3 CalculateSpotLight(SpotLight lightSource)
{
	return vec3(0);
}

float CalculateShadow()
{
	vec3 projectiveCoords = frag_position_shadow.xyz / frag_position_shadow.w;
	vec3 normalizedCoords = projectiveCoords * 0.5 + 0.5;
	float shadowDepth = texture(u_ShadowMap, normalizedCoords.xy).z;
	float currentDepth = normalizedCoords.z;
	float shadow = currentDepth  - 0.005 > shadowDepth ? 0 : 1;
	if(currentDepth + 0.005 > 1) return 1;
	return shadow;
}

vec3 CalculateLights()
{
	vec3 ambient = vec3(0,0,0);
	vec3 light = vec3(0,0,0);

	for(int i = 0; i < u_DirectionLightCount; i++)
		light += CalculateDirectionLight(u_DirectionLights[i]);

	for(int i = 0; i < u_PointLightCount; i++)
		light += CalculatePointLight(u_PointLights[i]);

	for(int i = 0; i < u_SpotLightCount; i++)
		light += CalculateSpotLight(u_SpotLights[i]);

	return ambient + light * CalculateShadow();
}

void main()
{	
	vec3 light = CalculateLights();	
	if(u_UseTexture == 0)
	{
		out_color = vec4(u_Color, 1) * vec4(light, 1);
	}
	else if(u_UseTexture == 1)
	{
		out_color = texture(u_Texture, frag_texture) * vec4(light, 1);
	}
	
}