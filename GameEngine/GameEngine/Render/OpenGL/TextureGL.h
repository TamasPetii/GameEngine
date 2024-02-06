#pragma once
#include <GL/glew.h>
#include <functional>

constexpr auto defaultParamTextureFunction = [](GLuint textureID) -> void {
	glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
};

constexpr auto defaultClearTextureFunction = [](GLuint textureID) -> void {
};

constexpr auto defaultReadTextureFunction = [](GLuint textureID) -> void {
};

struct TextureSpecGL
{
	GLint   width = 0;
	GLint   height = 0;
	GLsizei layer = 1;
	GLenum textureType = GL_TEXTURE_2D;
	GLint  internalFormat = GL_RGBA8;
	GLuint format = GL_RGBA;
	GLuint type = GL_FLOAT;
	std::function<void(GLuint)> paramTextureFunction = defaultParamTextureFunction;
	std::function<void(GLuint)> clearTextureFunction = defaultClearTextureFunction;
	std::function<void(GLuint)> readTextureFunction = defaultReadTextureFunction;
};

class TextureGL
{
public:
	TextureGL(const TextureSpecGL& spec);
	~TextureGL();
	void TextureSubImage(const void* data);
	void Regenerate(GLint width, GLint height);
	const GLuint GetTextureID() { return m_TextureID; }
private:
	GLuint m_TextureID;
	TextureSpecGL m_Specification;
};