#pragma once
#include "EngineApi.h"
#include <GL/glew.h>
#include <string>
#include <unordered_map>

class ShaderGL;

class ENGINE_API ProgramGL
{
public:
	ProgramGL(const std::vector<ShaderGL>& shaders);
	virtual ~ProgramGL();

	void Bind() const;
	void UnBind() const;

	template<typename T>
	void SetUniform(const std::string& name, const T& value);
	void SetTexture(const std::string& name, const int sampler, GLuint id);
private:
	GLuint m_ProgramID;
	GLint GetUniformLocation(const std::string& name);
	std::unordered_map<std::string, GLint> m_UniformLocations;
};

#include "ProgramGL.inl"

