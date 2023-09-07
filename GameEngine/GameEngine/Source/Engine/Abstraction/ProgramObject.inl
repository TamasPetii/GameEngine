#include "ProgramObject.h"

namespace OpenGL::Classic
{
	template<typename T>
	inline void ProgramObject::SetUniform(const std::string& name, const T& value)
	{
		throw std::runtime_error("Not supported shader uniform type!");
	}

	template<>
	inline void ProgramObject::SetUniform<GLint>(const std::string& name, const GLint& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform1i(location, value);
	}

	template<>
	inline void ProgramObject::SetUniform<GLuint>(const std::string& name, const GLuint& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform1ui(location, value);
	}

	template<>
	inline void ProgramObject::SetUniform<GLfloat>(const std::string& name, const GLfloat& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform1f(location, value);
	}

	template<>
	inline void ProgramObject::SetUniform<glm::vec2>(const std::string& name, const glm::vec2& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform2f(location, value[0], value[1]);
	}

	template<>
	inline void ProgramObject::SetUniform<glm::vec3>(const std::string& name, const glm::vec3& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform3f(location, value[0], value[1], value[2]);
	}

	template<>
	inline void ProgramObject::SetUniform<glm::vec4>(const std::string& name, const glm::vec4& value)
	{
		GLint location = GetUniformLocation(name);
		glUniform4f(location, value[0], value[1], value[2], value[3]);
	}

	template<>
	inline void ProgramObject::SetUniform<glm::mat4>(const std::string& name, const glm::mat4& value)
	{
		GLint location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, GL_FALSE, &(value[0][0]));
	}
}
