#version 460 core
#extension GL_ARB_bindless_texture : require
#define null 4294967295

layout(location = 0) in vec3 fs_in_pos;
layout(location = 1) in vec3 fs_in_norm;
layout(location = 2) in vec2 fs_in_tex;
layout(location = 3) in flat uvec3 fs_in_id; //Entity-Material-ShapeModel
layout(location = 4) in mat3 fs_in_tbn;

layout(location = 0) out vec4 fs_out_col;
layout(location = 1) out vec3 fs_out_add;
layout(location = 2) out vec3 fs_out_norm;
layout(location = 3) out uint fs_out_id;
layout(location = 4) out vec4 fs_out_bloom;
layout(location = 5) out vec3 fs_out_pos;

struct Material
{
    vec4 color;
    vec4 shinniness;
    uvec2 diffuseTexture;
    uvec2 normalTexture;
    uvec2 specularTexture;
    uvec2 fillter;
};

layout(std430, binding = 3) buffer u_materialData
{   
    Material materialData[];
};

layout(std430, binding = 4) buffer u_shapeModelData
{   
    bool shapeModelData[];
};

uniform uint u_renderMode;

void main()
{
    vec2 texcoord = fs_in_tex * vec2(materialData[fs_in_id.y].shinniness.zw);

    vec4 diffuseTextureColor = vec4(1);
    if(materialData[fs_in_id.y].diffuseTexture != uvec2(0))   
        diffuseTextureColor = texture(sampler2D(materialData[fs_in_id.y].diffuseTexture), texcoord);

    if(diffuseTextureColor.w < 0.05 || materialData[fs_in_id.y].color.w < 0.05)
        discard;

    float specularTextureColor = 1;
    if(materialData[fs_in_id.y].specularTexture != uvec2(0))
        specularTextureColor = texture(sampler2D(materialData[fs_in_id.y].specularTexture), texcoord).r;

    vec3 normal = normalize(fs_in_norm);
    if(materialData[fs_in_id.y].normalTexture != uvec2(0))
    { 
        normal = texture(sampler2D(materialData[fs_in_id.y].normalTexture), texcoord).xyz;
        normal = normal * 2.0 - 1.0;   
        normal = normalize(fs_in_tbn * normal); 
    }

    fs_out_col = materialData[fs_in_id.y].color * diffuseTextureColor;
    fs_out_norm = normal;
    fs_out_add = vec3(specularTextureColor, materialData[fs_in_id.y].shinniness.x, shapeModelData[fs_in_id.z]);
    fs_out_id = fs_in_id.x;
    fs_out_bloom = materialData[fs_in_id.y].shinniness.y == 1 ? fs_out_col : vec4(0, 0, 0, 1);
    fs_out_pos = fs_in_pos;
}