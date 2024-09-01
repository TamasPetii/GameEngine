#version 460 core
#extension GL_ARB_bindless_texture : require

layout(location = 0) in vec3 fs_in_pos;
layout(location = 1) in vec3 fs_in_norm;
layout(location = 2) in vec2 fs_in_tex;
layout(location = 3) in flat uint fs_in_id;

layout(location = 0) out vec3 fs_out_col;

struct Material
{
    vec4 color;
    vec4 shinniness;
    vec4 scale;
    uvec2 diffuseTexture;
    uvec2 normalTexture;
    uvec2 specularTexture;
    uvec2 environmentTexture;
};

layout(std430, binding = 2) buffer u_materialData
{   
    Material materialData[];
};

uniform vec3 u_eye;
uniform vec3 lightDir = vec3(-0.65,-0.5,-0.82);
uniform vec3 lightColor = vec3(1, 1, 1);

void main()
{
    vec2 texcoord = fs_in_tex * vec2(materialData[fs_in_id].scale.xy);

    vec4 diffuseTextureColor = vec4(1);
    if(materialData[fs_in_id].diffuseTexture != uvec2(0))
        diffuseTextureColor = texture(sampler2D(materialData[fs_in_id].diffuseTexture), texcoord);

    vec3 color = materialData[fs_in_id].color.xyz * diffuseTextureColor.xyz;

    //Direction Light calculation
    vec3 to_light = normalize(-lightDir);
	vec3 to_eye   = normalize(u_eye.xyz - fs_in_pos);

	//Diffuse
	vec3 diffuse = lightColor * color * clamp(dot(fs_in_norm, to_light), 0, 1);

	//Specular
	vec3 reflection = normalize(reflect(-to_light, fs_in_norm));
	vec3 specular = lightColor * color * pow(clamp(dot(to_eye, reflection), 0, 1), 64);

    fs_out_col = vec3(diffuse + specular);
}