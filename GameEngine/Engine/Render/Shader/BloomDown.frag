#version 460 core

layout(location = 0) in vec2 fs_in_tex;
layout(location = 0) out vec4 fs_out_col;

uniform sampler2D srcTexture;
uniform vec2 srcResolution;

void main()
{
    vec2 srcTexelSize = 1.0 / srcResolution;
    float x = srcTexelSize.x;
    float y = srcTexelSize.y;

    vec3 a = texture(srcTexture, vec2(fs_in_tex.x - 2*x, fs_in_tex.y + 2*y)).rgb;
    vec3 b = texture(srcTexture, vec2(fs_in_tex.x,       fs_in_tex.y + 2*y)).rgb;
    vec3 c = texture(srcTexture, vec2(fs_in_tex.x + 2*x, fs_in_tex.y + 2*y)).rgb;

    vec3 d = texture(srcTexture, vec2(fs_in_tex.x - 2*x, fs_in_tex.y)).rgb;
    vec3 e = texture(srcTexture, vec2(fs_in_tex.x,       fs_in_tex.y)).rgb;
    vec3 f = texture(srcTexture, vec2(fs_in_tex.x + 2*x, fs_in_tex.y)).rgb;

    vec3 g = texture(srcTexture, vec2(fs_in_tex.x - 2*x, fs_in_tex.y - 2*y)).rgb;
    vec3 h = texture(srcTexture, vec2(fs_in_tex.x,       fs_in_tex.y - 2*y)).rgb;
    vec3 i = texture(srcTexture, vec2(fs_in_tex.x + 2*x, fs_in_tex.y - 2*y)).rgb;

    vec3 j = texture(srcTexture, vec2(fs_in_tex.x - x, fs_in_tex.y + y)).rgb;
    vec3 k = texture(srcTexture, vec2(fs_in_tex.x + x, fs_in_tex.y + y)).rgb;
    vec3 l = texture(srcTexture, vec2(fs_in_tex.x - x, fs_in_tex.y - y)).rgb;
    vec3 m = texture(srcTexture, vec2(fs_in_tex.x + x, fs_in_tex.y - y)).rgb;

    vec3 color = e*0.125;
    color += (a+c+g+i)*0.03125;
    color += (b+d+f+h)*0.0625;
    color += (j+k+l+m)*0.125;

    fs_out_col = vec4(color, 1);
}