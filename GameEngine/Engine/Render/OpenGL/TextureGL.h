#pragma once
#include "EngineApi.h"
#include <GL/glew.h>
#include <iostream>
#include <functional>
#include <any>

constexpr auto defaultParamTextureFunction = [](GLuint textureID) -> void {
	glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
};

struct ENGINE_API TextureSpecGL
{
	bool generateMipMap{ true };
	bool generateHandler{ true };
	GLsizei layer{1};
	GLint width{0};
	GLint height{0};
	GLenum textureType;
	GLint  internalFormat;
	GLuint format;
	GLuint type;
	GLuint attachment;
	std::function<void(GLuint)> paramTextureFunction = defaultParamTextureFunction;
	std::function<void(GLuint, const TextureSpecGL&)> clearTextureFunction;
	std::function<std::any(GLuint, const TextureSpecGL&, GLint, GLint)> readTextureFunction;
};

class ENGINE_API TextureGL
{
public:
	TextureGL(const TextureSpecGL& spec);
	~TextureGL();
	void TextureSubImage2D(const void* data);
	void TextureSubImage3D(const void* data, int layer);
	void TextureCopy2D(unsigned int textureID);
	void GenerateMipMap();
	void GenerateHandler();

	void SetPath(const std::string& path) { this->path = path; }
	const std::string& GetPath() const { return path; }
	const GLuint GetTextureID() const { return m_TextureID; }
	const GLuint64 GetTextureHandler() const { return m_TextureHandler; }
	TextureSpecGL& GetTextureSpec() { return m_Specification; }
private:
	void MakeTextureHandlerResident() const { glMakeTextureHandleResidentARB(m_TextureHandler); }
	void MakeTextureNonHandlerResident() const { glMakeTextureHandleNonResidentARB(m_TextureHandler); }
private:
	std::string path;
	GLuint m_TextureID;
	GLuint64 m_TextureHandler;
	TextureSpecGL m_Specification;
};