#include "Program.h"

template<typename T>
inline void Program::SetUniform(const std::string& name, const T& value)
{
	throw std::runtime_error("Not supported shader uniform type!");
}

template<>
inline void Program::SetUniform<GLint>(const std::string& name, const GLint& value)
{
	glUniform1i(glGetUniformLocation(mProgramId, name.c_str()), value);
}

template<>
inline void Program::SetUniform<GLfloat>(const std::string& name, const GLfloat& value)
{
	glUniform1f(glGetUniformLocation(mProgramId, name.c_str()), value);
}

template<>
inline void Program::SetUniform<glm::vec2>(const std::string& name, const glm::vec2& value)
{
	glUniform2f(glGetUniformLocation(mProgramId, name.c_str()), value[0], value[1]);
}

template<>
inline void Program::SetUniform<glm::vec3>(const std::string& name, const glm::vec3& value)
{
	glUniform3f(glGetUniformLocation(mProgramId, name.c_str()), value[0], value[1], value[2]);
}

template<>
inline void Program::SetUniform<glm::vec4>(const std::string& name, const glm::vec4& value)
{
	glUniform4f(glGetUniformLocation(mProgramId, name.c_str()), value[0], value[1], value[2], value[3]);
}

template<>
inline void Program::SetUniform<glm::mat4>(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(mProgramId, name.c_str()), 1, GL_FALSE, &(value[0][0]));
}

inline void Program::SetUniformTexture(const std::string& name, const int sampler, const Texture* texture)
{
	if (texture == nullptr) return;
	glActiveTexture(GL_TEXTURE0 + sampler);
	glBindTexture(texture->GetType(), texture->GetTextureId());
	glUniform1i(glGetUniformLocation(mProgramId, name.c_str()), sampler);
}