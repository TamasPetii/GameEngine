#version 460 core

//Output Data
out int out_id;

//Uniforms
uniform int u_Id;

void main()
{	
	out_id = u_Id;
}
