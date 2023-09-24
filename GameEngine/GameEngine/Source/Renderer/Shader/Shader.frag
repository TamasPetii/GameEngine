#version 460 core
#define MAX_DIRECTION_LIGHT_COUNT 25
#define MAX_POINT_LIGHT_COUNT 25
#define MAX_SPOT_LIGHT_COUNT 25

layout(location = 0) out vec4 out_color;
layout(location = 1) out unsigned int out_id;

struct DirectionLight
{
	vec3 direction;
	vec3 color;
	float diffuse;
	float specular;
};

struct PointLight
{
	vec3 position;
	vec3 color;
	float diffuse;
	float specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	vec3 color;
	float diffuse;
	float specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Textures
{
	vec3 scale;
	int useDiffuse;
	int useSpecular;
	int useNormal;
	int useHeight;
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	sampler2D height;
};

//Input Data
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture;
in vec4 frag_position_shadow;
in mat3 frag_TBN;


//Uniforms
uniform Material u_Material;
uniform Textures u_Textures;
uniform uint u_Id;
uniform float heightScale;
uniform sampler2D u_ShadowMap;
uniform int u_CastShadows;
uniform vec3 u_CameraEye;
uniform vec3 u_Color;

uniform int u_DirectionLightCount = 0;
uniform DirectionLight u_DirectionLights[MAX_DIRECTION_LIGHT_COUNT];

uniform int u_PointLightCount = 0;
uniform PointLight u_PointLights[MAX_POINT_LIGHT_COUNT];

uniform int u_SpotLightCount = 0;
uniform SpotLight u_SpotLights[MAX_SPOT_LIGHT_COUNT];

uniform float textureScale;

vec3 CalculateDirectionLight(DirectionLight lightSource, vec3 normal)
{
    vec3 toEye = normalize(u_CameraEye - frag_position);
	vec3 toLight = normalize(-lightSource.direction);
	
	//Diffuse part
	float diffuseIntensity = clamp(dot(toLight, normal), 0, 1);
	vec3 diffuseColor = lightSource.color * lightSource.diffuse * u_Material.diffuse;
	vec3 diffuse = diffuseIntensity * diffuseColor;

	//Specular part
	vec3 specularReflection = normalize(reflect(-toLight, normal));
	float specularIntensity = pow(clamp(dot(toEye, specularReflection), 0, 1), 64);
	vec3 specularColor = lightSource.color * lightSource.specular * u_Material.specular;
	vec3 specular = specularIntensity * specularColor;

	return diffuse + specular;
}

vec3 CalculatePointLight(PointLight lightSource, vec3 normal)
{
	vec3 toEye = normalize(u_CameraEye - frag_position);
	vec3 toLight = normalize(lightSource.position - frag_position);
	float lightDistance = length(lightSource.position - frag_position);

	//Diffuse part
	float diffuseIntensity = clamp(dot(toLight, normal), 0, 1);
	vec3 diffuseColor = lightSource.color * lightSource.diffuse * u_Material.diffuse;
	vec3 diffuse = diffuseIntensity * diffuseColor * 1 / sqrt(lightDistance);

	//Specular part
	vec3 specularReflection = normalize(reflect(-toLight, normal));
	float specularVolume = pow(clamp(dot(toEye, specularReflection), 0, 1), 64);
	vec3 specularColor = lightSource.color * lightSource.specular * u_Material.specular;
	vec3 specular = specularVolume * specularColor * 1 / sqrt(lightDistance);

	return diffuse + specular;
}

vec3 CalculateSpotLight(SpotLight lightSource)
{
	return vec3(0);
}

float CalculateShadow()
{
	float bias = 0.005;
	vec3 projectiveCoords = frag_position_shadow.xyz / frag_position_shadow.w;
	vec3 normalizedCoords = projectiveCoords * 0.5 + 0.5;
	float currentDepth = normalizedCoords.z;
	
	int count = 0;
	float shadow = 0.0; 
	vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			count++;
			float pcfDepth = texture(u_ShadowMap, normalizedCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1 : 0.0;        
		}    
	}
	shadow /= count;

	if(normalizedCoords.z > 1) 
		shadow = 0.0;

	return 1.0 - shadow;
}

vec3 CalculateLights(vec3 normal)
{
	vec3 light = vec3(0,0,0);
	vec3 ambient = vec3(0.05,0.05,0.05) * u_Material.ambient;

	for(int i = 0; i < u_DirectionLightCount; i++)
		light += CalculateDirectionLight(u_DirectionLights[i], normal);

	for(int i = 0; i < u_PointLightCount; i++)
		light += CalculatePointLight(u_PointLights[i], normal);

	for(int i = 0; i < u_SpotLightCount; i++)
		light += CalculateSpotLight(u_SpotLights[i]);

	return ambient + light * (u_CastShadows != 0 ? CalculateShadow() : 1);
}

void main()
{	
	vec3 normal = normalize(frag_normal);
	vec2 fragCoords = frag_texture;

	if(u_Textures.useHeight != 0)
	{
		vec3 frag_tangent_position = frag_TBN * frag_position;
		vec3 camera_tangent_position = frag_TBN * u_CameraEye;
		vec3 toEye = normalize(camera_tangent_position - frag_tangent_position);

	    float height = texture(u_Textures.height, fract(vec2(fragCoords.x * u_Textures.scaleX, fragCoords.y * u_Textures.scaleY) * u_Textures.scale)).r;
		vec2 scaledToEye = toEye.xy * height * heightScale;
		fragCoords -= scaledToEye;

		if(fragCoords.x > 1.0 || fragCoords.y > 1.0 || fragCoords.x < 0.0 || fragCoords.y < 0.0)
			discard;

	}	

	if(u_Textures.useNormal != 0)
	{
		normal = texture(u_Textures.normal, fract(vec2(fragCoords.x * u_Textures.scaleX, fragCoords.y * u_Textures.scaleY) * u_Textures.scale)).rgb;
		normal = normal * 2 - 1;
		normal = normalize(frag_TBN * normal);
	}	


	out_color = vec4(1);

	if(u_Textures.useMain != 0)
		out_color *= texture(u_Textures.main, fract(vec2(fragCoords.x * u_Textures.scaleX, fragCoords.y * u_Textures.scaleY) * u_Textures.scale));
	out_color *= vec4(CalculateLights(normal), 1);

	out_id = u_Id;
}