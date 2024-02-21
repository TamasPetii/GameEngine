#pragma once
#include <GL/glew.h>
#include <iostream>
#include <functional>

constexpr auto defaultParamTextureFunction = [](GLuint textureID) -> void {
	glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
<<<<<<< HEAD
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
=======
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
};

struct TextureSpecGL
{
<<<<<<< HEAD
	bool generateMipMap{ true };
=======
	bool generateMipMap{ false };
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	bool generateHandler{ true };
	GLint width{0};
	GLint height{0};
	GLsizei layer{1};
	GLenum textureType;
	GLint  internalFormat;
	GLuint format;
	GLuint type;
	std::function<void(GLuint)> paramTextureFunction = defaultParamTextureFunction;
};

class TextureGL
{
public:
	TextureGL(const TextureSpecGL& spec);
	~TextureGL();
	void TextureSubImage2D(const void* data);
	void TextureSubImage3D(const void* data, int layer);

	const GLuint GetTextureID() const { return m_TextureID; }
	const GLuint64 GetTextureHandler() const { return m_TextureHandler; }
	void MakeTextureHandlerResident() const { glMakeTextureHandleResidentARB(m_TextureHandler); }
	void MakeTextureNonHandlerResident() const { glMakeTextureHandleNonResidentARB(m_TextureHandler); }
private:
	GLuint m_TextureID;
	GLuint64 m_TextureHandler;
	TextureSpecGL m_Specification;
};