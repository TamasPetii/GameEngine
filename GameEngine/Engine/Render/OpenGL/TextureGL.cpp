#include "TextureGL.h"

TextureGL::TextureGL(const TextureSpecGL& spec)
{
	glCreateTextures(spec.textureType, 1, &m_TextureID);

	if(spec.is2D)
		glTextureStorage2D(m_TextureID, 1, spec.internalFormat, spec.width, spec.height);
	else
		glTextureStorage3D(m_TextureID, 1, spec.internalFormat, spec.width, spec.height, spec.layer);

	spec.paramTextureFunction(m_TextureID);	
	m_Specification = spec;
}

TextureGL::~TextureGL()
{
	if(true || m_Specification.generateHandler)
		glMakeTextureHandleNonResidentARB(m_TextureID);

	glDeleteTextures(1, &m_TextureID);
}

void TextureGL::TextureCopy2D(unsigned int textureID)
{
	glCopyImageSubData(
		textureID, GL_TEXTURE_2D, 0, 0, 0, 0,
		m_TextureID, GL_TEXTURE_2D, 0, 0, 0, 0,
		m_Specification.width, m_Specification.height, 1
	);
}

void TextureGL::TextureSubImage2D(const void* data)
{
	glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Specification.width, m_Specification.height, m_Specification.format, m_Specification.type, data);

	GenerateMipMap();
	GenerateHandler();
}

void TextureGL::GenerateMipMap()
{
	if (m_Specification.generateMipMap)
	{
		glGenerateTextureMipmap(m_TextureID);
	}
}

void TextureGL::GenerateHandler()
{
	if (m_Specification.generateHandler)
	{
		m_TextureHandler = glGetTextureHandleARB(m_TextureID);
		glMakeTextureHandleResidentARB(m_TextureHandler);
	}
}

void TextureGL::TextureSubImage3D(const void* data, int layer)
{
	glTextureSubImage3D(m_TextureID, 0, 0, 0, layer, m_Specification.width, m_Specification.height, 1, m_Specification.format, m_Specification.type, data);
}
