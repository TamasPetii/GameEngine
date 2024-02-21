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
<<<<<<< HEAD
    vec4 scale;
=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
    uvec2 diffuseTexture;
    uvec2 normalTexture;
    uvec2 specularTexture;
    uvec2 environmentTexture;
};

layout(std430, binding = 1) buffer u_indexData
{   
    uvec4 indexData[]; //(EntityID, TransformID, MaterialID)
};

layout(std430, binding = 3) buffer u_materialData
{   
    Material materialData[];
};

void main()
{
<<<<<<< HEAD
    vec2 texcoord = fs_in_tex * vec2(materialData[indexData[fs_in_id].z].scale.xy);

    vec4 diffuseTextureColor = vec4(1);
    if(materialData[indexData[fs_in_id].z].diffuseTexture != uvec2(0))
        diffuseTextureColor = texture(sampler2D(materialData[indexData[fs_in_id].z].diffuseTexture), texcoord);

    float specularTextureColor = 1;
    if(materialData[indexData[fs_in_id].z].specularTexture != uvec2(0))
        specularTextureColor = texture(sampler2D(materialData[indexData[fs_in_id].z].specularTexture), texcoord).r;
=======
    vec4 diffuseTextureColor = vec4(1);
    if(materialData[indexData[fs_in_id].z].diffuseTexture != uvec2(0))
        diffuseTextureColor = texture(sampler2D(materialData[indexData[fs_in_id].z].diffuseTexture), fs_in_tex);

    float specularTextureColor = 1;
    if(materialData[indexData[fs_in_id].z].specularTexture != uvec2(0))
        specularTextureColor = texture(sampler2D(materialData[indexData[fs_in_id].z].specularTexture), fs_in_tex).r;
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

    vec3 normal = normalize(fs_in_norm);
    if(materialData[indexData[fs_in_id].z].normalTexture != uvec2(0))
    { 
<<<<<<< HEAD
        normal = texture(sampler2D(materialData[indexData[fs_in_id].z].normalTexture), texcoord).xyz;
=======
        normal = texture(sampler2D(materialData[indexData[fs_in_id].z].normalTexture), fs_in_tex).xyz;
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
        normal = normal * 2.0 - 1.0;   
        normal = normalize(fs_in_tbn * normal); 
    }

    fs_out_col = materialData[indexData[fs_in_id].z].color * diffuseTextureColor;
    fs_out_norm = normal;
    fs_out_add = vec3(specularTextureColor, materialData[indexData[fs_in_id].z].shinniness.x, 1);
    fs_out_id = uvec4(indexData[fs_in_id].xyz, null);
    fs_out_bloom = materialData[indexData[fs_in_id].z].shinniness.y == 1 ? fs_out_col : vec4(0, 0, 0, 1);
}