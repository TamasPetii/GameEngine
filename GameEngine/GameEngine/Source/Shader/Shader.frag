#version 460 core

struct DirectionalLight
{
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

//Output Data
out vec4 out_color;

//Uniforms
uniform vec3 u_CameraEye;
uniform vec3 u_Color;
uniform int u_UseTexture;
uniform sampler2D u_Texture;
uniform DirectionalLight u_DirectionalLight;

vec3 CalculateDirectionLight()
{
    vec3 toEye = normalize(u_CameraEye - frag_position);
	vec3 toLight = normalize(-u_DirectionalLight.direction);
	vec3 normal = normalize(frag_normal);
	
	//Diffuse part
	float diffuseVolume = clamp(dot(toLight, normal), 0, 1);
	vec3 diffuseColor = u_DirectionalLight.color * u_DirectionalLight.diffuse * u_DirectionalLight.volume;
	vec3 diffuse = diffuseVolume * diffuseColor;

	//Specular part
	vec3 specularReflection = normalize(reflect(-toLight, normal));
	float specularVolume = pow(clamp(dot(toEye, specularReflection), 0, 1), 64);
	vec3 specularColor = u_DirectionalLight.color * u_DirectionalLight.specular * u_DirectionalLight.volume;
	vec3 specular = specularVolume * specularColor;

	return diffuse + specular;
}

void main()
{	
	vec3 ambient = vec3(0, 0, 0);
	vec3 directionLightColor = CalculateDirectionLight();
	if(u_UseTexture == 0) out_color = vec4(u_Color, 1) * vec4(ambient + directionLightColor, 1);
	if(u_UseTexture == 1) out_color = texture(u_Texture, frag_texture) * vec4(directionLightColor, 1);
}