#include "TextureGL.h"

TextureGL::TextureGL(const TextureSpecGL& spec)
{
	m_Specification = spec;
	glCreateTextures(m_Specification.textureType, 1, &m_TextureID);
	glTextureStorage2D(m_TextureID, m_Specification.layer, m_Specification.internalFormat, m_Specification.width, m_Specification.height);
	m_Specification.paramTextureFunction(m_TextureID);
}

TextureGL::~TextureGL()
{
	glDeleteTextures(1, &m_TextureID);
}

void TextureGL::TextureSubImage(const void* data)
{
	glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Specification.width, m_Specification.height, m_Specification.format, m_Specification.type, data);
}

void TextureGL::Regenerate(GLint width, GLint height)
{
	m_Specification.width = width;
	m_Specification.height = height;
	glDeleteTextures(1, &m_TextureID);
	glCreateTextures(m_Specification.textureType, 1, &m_TextureID);
	glTextureStorage2D(m_TextureID, m_Specification.layer, m_Specification.internalFormat, m_Specification.width, m_Specification.height);
	m_Specification.paramTextureFunction(m_TextureID);
}
