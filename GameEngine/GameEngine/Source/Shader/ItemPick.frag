#version 460 core

//Output Data
out unsigned int out_id;

//Uniforms
uniform unsigned int u_Id;

void main()
{	
	out_id = u_Id;
}
