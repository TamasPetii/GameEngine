#include "TextureGL.h"

TextureGL::TextureGL(const TextureSpecGL& spec)
{
	glCreateTextures(spec.textureType, 1, &m_TextureID);
	glTextureStorage2D(m_TextureID, spec.layer, spec.internalFormat, spec.width, spec.height);
	spec.paramTextureFunction(m_TextureID);
	m_Specification = spec;
}

TextureGL::~TextureGL()
{
	glMakeTextureHandleNonResidentARB(m_TextureID);
	glDeleteTextures(1, &m_TextureID);
}

void TextureGL::TextureSubImage2D(const void* data)
{
	glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Specification.width, m_Specification.height, m_Specification.format, m_Specification.type, data);

	if (m_Specification.generateMipMap)
	{
		glGenerateTextureMipmap(m_TextureID);
	}

	if (m_Specification.generateHandler)
	{
		m_TextureHandler = glGetTextureHandleARB(m_TextureID);
		glMakeTextureHandleResidentARB(m_TextureHandler);
		std::cout << "Generated Texture handler " << m_TextureHandler << std::endl;
	}
}

void TextureGL::TextureSubImage3D(const void* data, int layer)
{
	glTextureSubImage3D(m_TextureID, 0, 0, 0, layer, m_Specification.width, m_Specification.height, 1, m_Specification.format, m_Specification.type, data);
}
