#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include <Render/OpenGL/ShaderGL.h>
#include <Logger/Logger.h>

class ProgramGL
{
public:
	ProgramGL(const std::vector<ShaderGL>& shaders);
	~ProgramGL();

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

