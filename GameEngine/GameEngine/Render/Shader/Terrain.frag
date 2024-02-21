#version 460 core

//Input Data
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture;

//Output Data
layout(location = 0) out vec4 out_color;
layout(location = 1) out unsigned int out_id;

uniform uint u_Id;
uniform sampler2D u_HeightMap;

void main()
{	
	//vec3 color = mix(vec3(1,1,1), vec3(0,1,0), clamp(dot(vec3(0,1,0), frag_normal), 0, 1));

	vec3 color = vec3(0.1, 1, 0.3);

	if(texture(u_HeightMap, frag_texture).r != 0)
	{
		float diffuse = clamp(dot(vec3(0,1,0), frag_normal), 0, 1);

		if(abs(diffuse - 1) < 0.1)
			color = vec3(1, 1, 1) * diffuse;
		else
			color = vec3(0.3, 0.3, 0.3) * diffuse;	
	}
	else
	{
		color = abs(vec3(int(frag_position.x / 2.f) / 100.f, int(frag_position.z / 2.f) / 100.f, 0));
	}


	out_id = u_Id;
	out_color = vec4(color, 1);
}
