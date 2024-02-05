#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include <Logger/Logger.h>

class ShaderGL
{
public:
	ShaderGL(GLenum type, const std::string& path);
	~ShaderGL();
	const auto& GetShaderID() const { return m_ShaderID; }
private:
	GLuint m_ShaderID;
};

