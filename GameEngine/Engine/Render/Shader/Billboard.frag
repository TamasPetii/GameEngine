#version 460 core

layout(location = 0) in vec2 fs_in_tex;
layout(location = 1) in flat uint fs_in_id;
layout(location = 0) out vec4 fs_out_col;
layout(location = 1) out uint fs_out_id;

uniform sampler2D billboardTexture;

void main()
{
	vec4 color = texture(billboardTexture, fs_in_tex);
	
	if(color.w < 0.05)
		discard;

	fs_out_col = color;
	fs_out_id = fs_in_id;
}