#version 460 core

layout(location = 0) in vec2 fs_in_tex;
layout(location = 0) out vec4 fs_out_col;

uniform sampler2D srcTexture;
uniform vec2 srcResolution;
uniform float filterRadius;

void main()
{
    float x = filterRadius;
    float y = filterRadius;

    vec3 a = texture(srcTexture, vec2(fs_in_tex.x - x, fs_in_tex.y + y)).rgb;
    vec3 b = texture(srcTexture, vec2(fs_in_tex.x,     fs_in_tex.y + y)).rgb;
    vec3 c = texture(srcTexture, vec2(fs_in_tex.x + x, fs_in_tex.y + y)).rgb;

    vec3 d = texture(srcTexture, vec2(fs_in_tex.x - x, fs_in_tex.y)).rgb;
    vec3 e = texture(srcTexture, vec2(fs_in_tex.x,     fs_in_tex.y)).rgb;
    vec3 f = texture(srcTexture, vec2(fs_in_tex.x + x, fs_in_tex.y)).rgb;

    vec3 g = texture(srcTexture, vec2(fs_in_tex.x - x, fs_in_tex.y - y)).rgb;
    vec3 h = texture(srcTexture, vec2(fs_in_tex.x,     fs_in_tex.y - y)).rgb;
    vec3 i = texture(srcTexture, vec2(fs_in_tex.x + x, fs_in_tex.y - y)).rgb;

    vec3 color = e*4.0;
    color += (b+d+f+h)*2.0;
    color += (a+c+g+i);
    color *= 1.0 / 16.0;

    fs_out_col = vec4(color, 1);
}