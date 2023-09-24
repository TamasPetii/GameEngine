#include "ShaderObject.h"
#include <iostream>
namespace OpenGL
{
	ShaderObject::ShaderObject(GLuint type, const std::string& path)
	{
		std::cout << "Shader: " << path << std::endl;

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
		m_ShaderId = glCreateShader(type);
		if (m_ShaderId == 0)
		{
			std::string strType = (type == GL_VERTEX_SHADER ? "vertex" : type == GL_GEOMETRY_SHADER ? "geometry" : "fragment");
			throw std::runtime_error("Error occurred while creating shader.\nType: " + strType + "\nPath: " + path);
		}

		//Attaching the content of shader file
		const char* sourcePointer = source.c_str();
		glShaderSource(m_ShaderId, 1, &sourcePointer, nullptr);
		glCompileShader(m_ShaderId);

		//Check possible shader compile errors
		int result, info_length;
		glGetShaderiv(m_ShaderId, GL_COMPILE_STATUS, &result);
		glGetShaderiv(m_ShaderId, GL_INFO_LOG_LENGTH, &info_length);

		if (result == GL_FALSE)
		{
			std::vector<GLchar> message(info_length);
			glGetShaderInfoLog(m_ShaderId, info_length, &info_length, message.data());
			glDeleteShader(m_ShaderId);

			std::string strType = (type == GL_VERTEX_SHADER ? "vertex" : type == GL_GEOMETRY_SHADER ? "geometry" : "fragment");
			throw std::runtime_error("Error occurred while compiling shaders.\nType: " + strType + "\nPath: " + path + "\nMessage: " + message.data());
		}
	}

	ShaderObject::~ShaderObject()
	{
		glDeleteShader(m_ShaderId);
	}
}