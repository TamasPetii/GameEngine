#include "ShaderGL.h"

ShaderGL::ShaderGL(GLenum type, const std::string& path)
{
	std::string typeName = "UNDEFINED";

	switch (type)
	{
	case GL_VERTEX_SHADER: typeName = "VERTEX"; break;
	case GL_GEOMETRY_SHADER: typeName = "GEOMETRY"; break;
	case GL_FRAGMENT_SHADER: typeName = "FRAGMENT"; break;
	case GL_COMPUTE_SHADER: typeName = "COMPUTE"; break;
	case GL_TESS_CONTROL_SHADER: typeName = "TESSCONTROL"; break;
	case GL_TESS_EVALUATION_SHADER: typeName = "TESSEVAL"; break;
	default: break;
	}

	std::ifstream f(path);
	if (!f.is_open())
		throw std::runtime_error("Cannot open shader file.\nPath: " + path);

	//Process the content of shader file
	std::string line, source;
	while (std::getline(f, line))
	{
		source += line + "\n";
	}

	m_ShaderID = glCreateShader(type);
	if (m_ShaderID == 0)
	{
		std::stringstream sstream;
		sstream << "Error occurred while creating shader." << std::endl;
		sstream << "Type: " + typeName << std::endl;
		sstream << "Path : " + path << std::endl;
		throw std::runtime_error(sstream.str());
	}

	//Attaching the content of shader file
	const char* sourcePointer = source.c_str();
	glShaderSource(m_ShaderID, 1, &sourcePointer, nullptr);
	glCompileShader(m_ShaderID);

	//Check possible shader compile errors
	int result, info_length;
	glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(m_ShaderID, GL_INFO_LOG_LENGTH, &info_length);

	if (result == GL_FALSE)
	{
		std::vector<GLchar> message(info_length);
		glGetShaderInfoLog(m_ShaderID, info_length, &info_length, message.data());
		glDeleteShader(m_ShaderID);

		std::stringstream sstream;
		sstream << "Error occurred while compiling shaders." << std::endl;
		sstream << "Type: " + typeName << std::endl;
		sstream << "Path : " + path << std::endl;
		sstream << "Message: " << message.data() << std::endl;
		throw std::runtime_error(sstream.str());
	}

	Logger<INIT>::Log() << "[ShaderGL] : Shader created successfully. ID = " << m_ShaderID << std::endl;
}

ShaderGL::~ShaderGL()
{
	Logger<INIT>::Log() << "[ShaderGL] : Shader deleted successfully. ID = " << m_ShaderID << std::endl;
	glDeleteShader(m_ShaderID);
}
