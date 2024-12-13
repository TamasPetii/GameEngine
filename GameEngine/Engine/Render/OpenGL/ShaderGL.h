#pragma once
#include "EngineApi.h"
#include <GL/glew.h>
#include <string>

class ENGINE_API ShaderGL
{
public:
	ShaderGL(GLenum type, const std::string& path);
	~ShaderGL();
	const auto& GetShaderID() const { return m_ShaderID; }
private:
	GLuint m_ShaderID;
};

