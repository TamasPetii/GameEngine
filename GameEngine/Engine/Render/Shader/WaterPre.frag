#version 460 core
#extension GL_ARB_bindless_texture : require

layout(location = 0) in vec3 fs_in_pos;
layout(location = 1) in vec3 fs_in_norm;
layout(location = 2) in vec2 fs_in_tex;
layout(location = 3) in flat uint fs_in_matID;

layout(location = 0) out vec4 fs_out_col;

struct Material
{
    vec4 color;
    vec4 shinniness;
    uvec2 diffuseTexture;
    uvec2 normalTexture;
    uvec2 specularTexture;
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

layout(std430, binding = 4) buffer u_materialData
{   
    Material materialData[];
};

uniform uint u_renderMode;
uniform vec3 lightDir = vec3(-0.23,-0.5,-0.82);
uniform vec3 lightColor = vec3(1, 1, 1);
uniform samplerCube u_skyboxTexture;

void main()
{
    //Skybox
    if(u_renderMode == 4)
    {
        fs_out_col = texture(u_skyboxTexture, fs_in_pos);
        return;
    }

    vec2 texcoord = fs_in_tex * vec2(materialData[fs_in_matID].shinniness.zw);

    vec4 diffuseTextureColor = vec4(1);
    if(materialData[fs_in_matID].diffuseTexture != uvec2(0))
        diffuseTextureColor = texture(sampler2D(materialData[fs_in_matID].diffuseTexture), texcoord);
    
    vec3 color = (materialData[fs_in_matID].color * diffuseTextureColor).xyz;
    
    vec3 to_light = normalize(-lightDir);
	vec3 to_eye   = normalize(eye.xyz - fs_in_pos);

	//Diffuse
	vec3 diffuse = lightColor * color * clamp(dot(fs_in_norm, to_light), 0, 1);

	//Specular
	vec3 reflection = normalize(reflect(-to_light, fs_in_norm));
	vec3 specular = lightColor * color * pow(clamp(dot(to_eye, reflection), 0, 1), 64);

    fs_out_col = vec4(diffuse + specular, 1);
}