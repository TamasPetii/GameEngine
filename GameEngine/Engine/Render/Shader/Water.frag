#version 460 core
#extension GL_ARB_bindless_texture : require
#define null 4294967295

layout(location = 0) in vec3 fs_in_pos;
layout(location = 1) in vec3 fs_in_norm;
layout(location = 2) in vec4 fs_in_clip;
layout(location = 3) in vec2 fs_in_tex;

layout(location = 0) out vec4 fs_out_col;
layout(location = 1) out uvec4 fs_out_id;

struct Water
{
    vec4 plane;
    vec2 scale;
    vec2 waveStrength;
    vec2 waveMove;
    uvec2 dudvTexture;
};

layout(std430, binding = 2) buffer u_waterData
{
    Water water[];
};

uniform uint u_entityIndex;
uniform uint u_entityID;

uniform sampler2D u_reflection;
uniform sampler2D u_refraction;

void main()
{;
    
    vec2 dudv = vec2(0);
    if(water[u_entityIndex].dudvTexture != 0)
    {
        vec2 dudv1 = vec2(texture(sampler2D(water[u_entityIndex].dudvTexture), fs_in_tex * water[u_entityIndex].scale + vec2(water[u_entityIndex].waveMove.x, 0)).xy * 2 - 1) * water[u_entityIndex].waveStrength;
        vec2 dudv2 = vec2(texture(sampler2D(water[u_entityIndex].dudvTexture), fs_in_tex * vec2(-1, 1) * water[u_entityIndex].scale + vec2(water[u_entityIndex].waveMove.x)).xy * 2 - 1) * water[u_entityIndex].waveStrength;
        dudv = dudv1 + dudv2;
    }

    vec2 ndc = vec2((fs_in_clip.xy / fs_in_clip.w) / 2.0 + 0.5);
    vec2 reflect_tex = vec2(ndc.x, 1 - ndc.y);
    vec2 refract_tex = vec2(ndc.x, ndc.y);

    reflect_tex += dudv;
    refract_tex += dudv;

    vec4 reflectColor = texture(u_reflection, reflect_tex);
    vec4 refractColor = texture(u_refraction, refract_tex);

    fs_out_col = mix(reflectColor, refractColor, 0.5);
    fs_out_id = uvec4(u_entityID, null, null, null);
}