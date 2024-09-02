#include "ProgramGL.h"

ProgramGL::ProgramGL(const std::vector<ShaderGL>& shaders)
{
	m_ProgramID = glCreateProgram();

	for (auto& shader : shaders)
		glAttachShader(m_ProgramID, shader.GetShaderID());

	glLinkProgram(m_ProgramID);
	glValidateProgram(m_ProgramID);

	//Check possible linking errors
	int result, info_length;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &info_length);

	if (result == GL_FALSE)
	{
		std::vector<GLchar> message(info_length);
		glGetProgramInfoLog(m_ProgramID, info_length, &info_length, message.data());
		throw std::runtime_error("Error occurred while linking program.\nId: " + std::to_string(m_ProgramID) + "\nMessage: " + std::string(message.data()));
	}
}

ProgramGL::~ProgramGL()
{
	glDeleteProgram(m_ProgramID);
}

void ProgramGL::Bind() const
{
	glUseProgram(m_ProgramID);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ProgramGL::UnBind() const
{
	glUseProgram(0);
}

void ProgramGL::SetTexture(const std::string& name, const int sampler, GLuint id)
{
	GLint location = GetUniformLocation(name);
	glBindTextureUnit(sampler, id);
	glUniform1i(location, sampler);
}

GLint ProgramGL::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocations.find(name) == m_UniformLocations.end())
	{
		m_UniformLocations[name] = glGetUniformLocation(m_ProgramID, name.c_str());
	}

	return m_UniformLocations[name];
}
