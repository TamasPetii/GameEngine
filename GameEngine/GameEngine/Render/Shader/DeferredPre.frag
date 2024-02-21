#version 460 core
#extension GL_ARB_bindless_texture : require
#define null 4294967295

layout(location = 0) in vec3 fs_in_pos;
layout(location = 1) in vec3 fs_in_norm;
layout(location = 2) in vec2 fs_in_tex;
layout(location = 3) in mat3 fs_in_tbn;

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

layout(std430, binding = 2) buffer u_materialData
{   
    Material materialData[];
};

uniform uint u_transformIndex;
uniform uint u_materialIndex;
uniform uint u_entityIndex;

void main()
{
<<<<<<< HEAD
    vec2 texcoord = fs_in_tex * vec2(materialData[u_materialIndex].scale.xy);

    vec4 diffuseTextureColor = vec4(1);
    if(materialData[u_materialIndex].diffuseTexture != uvec2(0))
        diffuseTextureColor = texture(sampler2D(materialData[u_materialIndex].diffuseTexture), texcoord);

    float specularTextureColor = 1;
    if(materialData[u_materialIndex].specularTexture != uvec2(0))
        specularTextureColor = texture(sampler2D(materialData[u_materialIndex].specularTexture), texcoord).r;
=======
    //Use material index not common index and transform as well

    vec4 diffuseTextureColor = vec4(1);
    if(materialData[u_materialIndex].diffuseTexture != uvec2(0))
        diffuseTextureColor = texture(sampler2D(materialData[u_materialIndex].diffuseTexture), fs_in_tex);

    float specularTextureColor = 1;
    if(materialData[u_materialIndex].specularTexture != uvec2(0))
        specularTextureColor = texture(sampler2D(materialData[u_materialIndex].specularTexture), fs_in_tex).r;
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

    vec3 normal = normalize(fs_in_norm);
    if(materialData[u_materialIndex].normalTexture != uvec2(0))
    { 
<<<<<<< HEAD
        normal = texture(sampler2D(materialData[u_materialIndex].normalTexture), texcoord).xyz;
=======
        normal = texture(sampler2D(materialData[u_materialIndex].normalTexture), fs_in_tex).xyz;
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
        normal = normal * 2.0 - 1.0;   
        normal = normalize(fs_in_tbn * normal); 
    }

    fs_out_col = materialData[u_materialIndex].color * diffuseTextureColor;
    fs_out_norm = normal;
    fs_out_add = vec3(specularTextureColor, materialData[u_materialIndex].shinniness.x, 1);
    fs_out_id = uvec4(u_entityIndex, null, null, null);
    fs_out_bloom = materialData[u_materialIndex].shinniness.y == 1 ? fs_out_col : vec4(0, 0, 0, 1);
}