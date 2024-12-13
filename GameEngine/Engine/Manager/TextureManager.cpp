#include "TextureManager.h"
#include <algorithm>
#include <filesystem>
#include <stb_image.h>

#include "Logger/Logger.h"
#include "Render/OpenGL/TextureGL.h"
#include "Settings/GlobalSettings.h"

TextureManager* TextureManager::m_Instance = nullptr;

TextureManager* TextureManager::Instance()
{
	if (m_Instance == nullptr)
		m_Instance = new TextureManager();
	return m_Instance;
}

TextureManager::~TextureManager()
{
}

void TextureManager::Destroy()
{
	if (m_Instance != nullptr)
		delete m_Instance;
	m_Instance = nullptr;
}

std::shared_ptr<TextureGL> TextureManager::LoadImageTexture(const std::string& path)
{
	std::string texturePath = path;

	//This is for loading pc path free assets
	if (std::filesystem::exists(GlobalSettings::ProjectPath + "/" + path))
		texturePath = GlobalSettings::ProjectPath + "/" + path;

	if (m_Textures.find(texturePath) == m_Textures.end() || (m_Textures.find(texturePath) != m_Textures.end() && m_Textures[texturePath] == nullptr))
	{
		int width, height, bpp;
		stbi_set_flip_vertically_on_load(true);
		GLubyte* data = stbi_load(texturePath.c_str(), &width, &height, &bpp, 0);

		if (data)
		{
			GLenum format = 0;
			GLenum internalFormat = 0;

			switch (bpp)
			{
			case 1: format = GL_RED;  internalFormat = GL_R8;    break;
			case 2: format = GL_RG;   internalFormat = GL_RG8;   break;
			case 3: format = GL_RGB;  internalFormat = GL_RGB8;  break;
			case 4: format = GL_RGBA; internalFormat = GL_RGBA8; break;
			default:
				break;
			}

			TextureSpecGL spec;
			spec.textureType = GL_TEXTURE_2D;
			spec.format = format;
			spec.internalFormat = internalFormat;
			spec.type = GL_UNSIGNED_BYTE;
			spec.width = width;
			spec.height = height;

			auto texture = std::make_shared<TextureGL>(spec);
			texture->TextureSubImage2D(data);
			texture->SetPath(texturePath);
			m_Textures[texturePath] = texture;

			LOG_DEBUG("TextureManager", "Image texture successfully loaded: " + texturePath);
		}
		else
		{
			m_Textures[texturePath] = nullptr;
			LOG_ERROR("TextureManager", "Couldn't load Image texture: " + texturePath);
		}

		stbi_image_free(data);
	}

	return m_Textures[texturePath];
}

std::shared_ptr<TextureGL> TextureManager::LoadImageTextureMap(const std::string& path)
{
	std::string texturePath = path;

	//This is for loading pc path free assets
	if (std::filesystem::exists(GlobalSettings::ProjectPath + "/" + path))
		texturePath = GlobalSettings::ProjectPath + "/" + path;

	if (m_TexturesMaps.find(texturePath) == m_TexturesMaps.end() || (m_Textures.find(texturePath) != m_Textures.end() && m_Textures[texturePath] == nullptr))
	{
		int width, height, bpp;
		stbi_set_flip_vertically_on_load(true);
		GLubyte* data = stbi_load(texturePath.c_str(), &width, &height, &bpp, 0);

		if (data)
		{
			GLenum format = 0;
			GLenum internalFormat = 0;

			switch (bpp)
			{
			case 1: format = GL_RED;  internalFormat = GL_R8;    break;
			case 2: format = GL_RG;   internalFormat = GL_RG8;   break;
			case 3: format = GL_RGB;  internalFormat = GL_RGB8;  break;
			case 4: format = GL_RGBA; internalFormat = GL_RGBA8; break;
			default:
				break;
			}

			constexpr auto paramTextureFunction = [](unsigned int textureID) -> void {
				glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTextureParameteri(textureID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				};

			TextureSpecGL spec;
			spec.textureType = GL_TEXTURE_CUBE_MAP;
			spec.format = format;
			spec.internalFormat = internalFormat;
			spec.type = GL_UNSIGNED_BYTE;
			spec.width = width / 4;
			spec.height = height / 3;
			spec.paramTextureFunction = paramTextureFunction;
			auto texture = std::make_shared<TextureGL>(spec);
			texture->SetPath(texturePath);

			int chunkWidth = width / 4;
			int chunkHeight = height / 3;

			for (int side = 0; side < 6; side++)
			{
				std::vector<unsigned char> chunkData(sizeof(unsigned char) * chunkWidth * chunkHeight * bpp);

				for (unsigned int h = 0; h < chunkHeight; h++)
				{
					int offsetHeight;
					int offsetWidth;

					switch (side)
					{
					case 0:
						offsetHeight = 1 * chunkHeight;
						offsetWidth = 2 * chunkWidth;
						break;
					case 1:
						offsetHeight = 1 * chunkHeight;
						offsetWidth = 0 * chunkWidth;
						break;
					case 2:
						offsetHeight = 0 * chunkHeight;
						offsetWidth = 1 * chunkWidth;
						break;
					case 3:
						offsetHeight = 2 * chunkHeight;
						offsetWidth = 1 * chunkWidth;
						break;
					case 4:
						offsetHeight = 1 * chunkHeight;
						offsetWidth = 1 * chunkWidth;
						break;
					case 5:
						offsetHeight = 1 * chunkHeight;
						offsetWidth = 3 * chunkWidth;
						break;
					}

					int newDataIndex = h * chunkWidth * bpp;
					int dataIndex = (offsetWidth + (h + offsetHeight) * width) * bpp;
					std::memcpy(&chunkData[newDataIndex], &data[dataIndex], bpp * chunkWidth);
				}

				texture->TextureSubImage3D(chunkData.data(), side);
			}

			m_TexturesMaps[texturePath] = texture;
			LoadImageTexture(texturePath);

			LOG_DEBUG("TextureManager", "CubeMap texture successfully loaded: " + texturePath);
		}
		else
		{
			m_Textures[texturePath] = nullptr;
			LOG_ERROR("TextureManager", "Couldn't load CubeMap texture: " + texturePath);
		}

		stbi_image_free(data);
	}

	return m_TexturesMaps[texturePath];
}