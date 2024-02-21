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