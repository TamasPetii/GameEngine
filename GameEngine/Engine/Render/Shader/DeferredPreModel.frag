#version 460 core
#extension GL_ARB_bindless_texture : require
#define null 4294967295

layout(location = 0) in vec3 fs_in_pos;
layout(location = 1) in vec3 fs_in_norm;
layout(location = 2) in vec2 fs_in_tex;
layout(location = 3) in flat uint fs_in_id;
layout(location = 4) in mat3 fs_in_tbn;

layout(location = 0) out vec4 fs_out_col;
layout(location = 1) out vec3 fs_out_add;
layout(location = 2) out vec3 fs_out_norm;
layout(location = 3) out uvec4 fs_out_id;
layout(location = 4) out vec4 fs_out_bloom;

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
uniform uint u_entityIndex;

void main()
{
    vec2 texcoord = fs_in_tex * vec2(materialData[fs_in_id].scale.xy);

    vec4 diffuseTextureColor = vec4(1);
    if(materialData[fs_in_id].diffuseTexture != uvec2(0))
        diffuseTextureColor = texture(sampler2D(materialData[fs_in_id].diffuseTexture), texcoord);

    float specularTextureColor = 1;
    if(materialData[fs_in_id].specularTexture != uvec2(0))
        specularTextureColor = texture(sampler2D(materialData[fs_in_id].specularTexture), texcoord).r;

    vec3 normal = normalize(fs_in_norm);
    if(materialData[fs_in_id].normalTexture != uvec2(0))
    { 
        normal = texture(sampler2D(materialData[fs_in_id].normalTexture), texcoord).xyz;
        normal = normal * 2.0 - 1.0;   
        normal = normalize(fs_in_tbn * normal); 
    }

    fs_out_col = materialData[fs_in_id].color * diffuseTextureColor;
    fs_out_norm = normal;
    fs_out_add = vec3(specularTextureColor, materialData[fs_in_id].shinniness.x, 1);
    fs_out_id = uvec4(u_entityIndex, null, fs_in_id, null);
    fs_out_bloom = materialData[fs_in_id].shinniness.y == 1 ? fs_out_col : vec4(0, 0, 0, 1);
}