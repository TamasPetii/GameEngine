#pragma once
#include "EngineApi.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include <Logger/Logger.h>

class ENGINE_API ShaderGL
{
public:
	ShaderGL(GLenum type, const std::string& path);
	~ShaderGL();
	static void PreLoadShader(const std::string& path);
	const auto& GetShaderID() const { return m_ShaderID; }
private:
	GLuint m_ShaderID;
};

