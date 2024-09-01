#version 460 core
#extension GL_ARB_bindless_texture : require
#define null 4294967295

layout(location = 0) in vec3 fs_in_pos;
layout(location = 1) in vec3 fs_in_norm;
layout(location = 2) in vec2 fs_in_tex;
layout(location = 3) in mat3 fs_in_tbn;

layout(location = 0) out vec4 fs_out_col;

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

uniform uint u_transformIndex;
uniform uint u_materialIndex;
uniform uint u_entityIndex;

void main()
{
    vec2 texcoord = fs_in_tex * vec2(materialData[u_materialIndex].scale.xy);

    vec4 diffuseTextureColor = vec4(1);
    if(materialData[u_materialIndex].diffuseTexture != uvec2(0))
        diffuseTextureColor = texture(sampler2D(materialData[u_materialIndex].diffuseTexture), texcoord);

    float specularTextureColor = 1;
    if(materialData[u_materialIndex].specularTexture != uvec2(0))
        specularTextureColor = texture(sampler2D(materialData[u_materialIndex].specularTexture), texcoord).r;

    vec3 normal = normalize(fs_in_norm);
    if(materialData[u_materialIndex].normalTexture != uvec2(0))
    { 
        normal = texture(sampler2D(materialData[u_materialIndex].normalTexture), texcoord).xyz;
        normal = normal * 2.0 - 1.0;   
        normal = normalize(fs_in_tbn * normal); 
    }

    fs_out_col = materialData[u_materialIndex].color * diffuseTextureColor;
}