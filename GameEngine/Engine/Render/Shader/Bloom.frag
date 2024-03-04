#version 460 core

layout(location = 0) in vec2 fs_in_tex;
layout(location = 0) out vec4 fs_out_col;

uniform float gamma = 1.0;
uniform float exposure = 1.0;
uniform sampler2D bloomTexture;

void main()
{             
    vec3 hdrColor = texture(bloomTexture, fs_in_tex).rgb;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    mapped = pow(mapped, vec3(1.0 / gamma));

    fs_out_col = vec4(mapped, 1.0);
}  