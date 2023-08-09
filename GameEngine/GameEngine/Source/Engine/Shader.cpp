#include "Shader.h"

Shader::Shader(unsigned int type, const std::string& path)
{
	//Opening the shader file
	std::ifstream f(path);
	if (!f.is_open())
	{
		throw std::runtime_error("Cannot open shader file.\nPath: " + path);
	}

	//Process the content of shader file
	std::string line, source;
	while (std::getline(f, line))
	{
		source += line + "\n";
	}

	//Creating a new shader
	mShaderId = glCreateShader(type);
	if (mShaderId == 0)
	{
		std::string strType = (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		throw std::runtime_error("Error occurred while creating shader.\nType: " + strType + "\nPath: " + path);
	}

	//Attaching the content of shader file
	const char* sourcePointer = source.c_str();
	glShaderSource(mShaderId, 1, &sourcePointer, nullptr);
	glCompileShader(mShaderId);

	//Check possible shader compile errors
	int result, info_length;
	glGetShaderiv(mShaderId, GL_COMPILE_STATUS, &result);
	glGetShaderiv(mShaderId, GL_INFO_LOG_LENGTH, &info_length);

	if (result == GL_FALSE)
	{
		char* message = (char*)_malloca(info_length * sizeof(char));
		glGetShaderInfoLog(mShaderId, info_length, &info_length, message);
		glDeleteShader(mShaderId);
		
		std::string strType = (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		throw std::runtime_error("Error occurred while compiling shaders.\nType: " + strType + "\nPath: " + path + "\nMessage: " + message);
		
		delete message;
	}
}

Shader::~Shader()
{
	glDeleteShader(mShaderId);
}