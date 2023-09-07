#include "ProgramObject.h"

namespace OpenGL::Classic
{
	ProgramObject::ProgramObject()
	{

	}

	ProgramObject::ProgramObject(const std::vector<ShaderObject>& shaders, const std::vector<ShaderObjectInfo>& infos)
	{
		m_Shaders = shaders;
		m_ShaderInfos = infos;
		GenerateProgram();
	}

	ProgramObject::~ProgramObject()
	{
		DeleteProgram();
	}

	void ProgramObject::GenerateProgram()
	{
    	m_ProgramId = glCreateProgram();

		//Attach shader to program
		for (auto& shader : m_Shaders)
		{
			glAttachShader(m_ProgramId, shader.Get_ShaderId());
		}

		//Assigning VAO attributes to shader variables (Must do before linking the program!)
		for (auto& info : m_ShaderInfos)
		{
			glBindAttribLocation(m_ProgramId, info.Get_Location(), info.Get_Name().c_str());
		}

		//Linking the program and validating it -> information about the validation in info log
		glLinkProgram(m_ProgramId);
		glValidateProgram(m_ProgramId);

		//Check possible linking errors
		int result, info_length;
		glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &result);
		glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &info_length);

		if (result == GL_FALSE)
		{
			std::vector<GLchar> message(info_length);

			glGetProgramInfoLog(m_ProgramId, info_length, &info_length, message.data());

			throw std::runtime_error("Error occurred while linking the program.\Id: " + std::to_string(m_ProgramId) + "\nMessage: " + std::string(message.data()));
		}

		m_Shaders.clear();
		m_ShaderInfos.clear();

		glUseProgram(0);
	}

	void ProgramObject::DeleteProgram()
	{
		glDeleteProgram(m_ProgramId);
	}

	void ProgramObject::Bind() const
	{
		glUseProgram(m_ProgramId);
	}

	void ProgramObject::UnBind() const
	{
		glUseProgram(0);
	}

	void ProgramObject::AttachShader(const ShaderObject& shader)
	{
		m_Shaders.push_back(shader);
	}

	void ProgramObject::AttachShaderInfo(const ShaderObjectInfo& info)
	{
		m_ShaderInfos.push_back(info);
	}

	GLint ProgramObject::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocations.find(name) == m_UniformLocations.end())
		{
			m_UniformLocations[name] = glGetUniformLocation(m_ProgramId, name.c_str());
			std::cout << name << ": " << m_UniformLocations.at(name) << std::endl;
		}


		return m_UniformLocations.at(name);
	}

	void ProgramObject::SetUniformTexture(const std::string& name, const int sampler, GLuint id, GLuint type)
	{
		GLint location = GetUniformLocation(name);
		glActiveTexture(GL_TEXTURE0 + sampler);
		glBindTexture(type, id);
		glUniform1i(location, sampler);
	}

	void ProgramObject::SetUniformTexture(const std::string& name, const int sampler, const ImageTexture* texture)
	{
		if (texture == 0) return;
		SetUniformTexture(name, sampler, texture->Get_TextureId(), texture->Get_TextureType());
	}
}