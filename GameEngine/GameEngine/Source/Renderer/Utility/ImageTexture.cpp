#include "ImageTexture.h"

std::unordered_map<std::string, ImageTexture*> ImageTexture::m_LoadedTextures;

ImageTexture::~ImageTexture()
{
	glDeleteTextures(1, &m_TextureId);
}

ImageTexture* ImageTexture::LoadTexture(const std::string& path)
{
	if (m_LoadedTextures.find(path) == m_LoadedTextures.end())
	{
		int width, height, bpp;
		stbi_set_flip_vertically_on_load(true);
		GLubyte* data = stbi_load(path.c_str(), &width, &height, &bpp, 0);

		if (data)
		{
			GLenum format = bpp == 1 ? GL_RED : bpp == 3 ? GL_RGB : GL_RGBA;

			ImageTexture* texture = new ImageTexture(GL_TEXTURE_2D, path);
			glGenTextures(1, &texture->m_TextureId);
			glBindTexture(GL_TEXTURE_2D, texture->m_TextureId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			stbi_image_free(data);

			std::cout << "[Loaded Image] >> " + path << std::endl;

			m_LoadedTextures[path] = texture;
		}
		else
		{
			stbi_image_free(data);
			std::runtime_error("[ImageTexture::LoadImage] : Couldn't load image >> " + path);
		}
	}

	return m_LoadedTextures.at(path);
}

ImageTexture* ImageTexture::LoadTextureMap(const std::vector<std::string>& paths)
{
	std::string combinePath = CombinePaths(paths);

	if (m_LoadedTextures.find(combinePath) == m_LoadedTextures.end())
	{
		ImageTexture* texture = new ImageTexture(GL_TEXTURE_CUBE_MAP, combinePath);
		glGenTextures(1, &texture->m_TextureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->m_TextureId);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (int side = 0; side < 6; side++)
		{
			int width, height, bpp;
			stbi_set_flip_vertically_on_load(false);
			GLubyte* data = stbi_load(paths[side].c_str(), &width, &height, &bpp, 0);

			if (data)
			{
				GLint internalFormat = bpp == 3 ? GL_RGB : GL_RGBA;
				GLenum format = bpp == 3 ? GL_RGB : GL_RGBA;
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				stbi_image_free(data);
				std::runtime_error("[ImageTexture::LoadImageMap] : Couldn't load image >> " + paths[side]);
			}
		}

		m_LoadedTextures[combinePath] = texture;
	}

	return m_LoadedTextures.at(combinePath);
}

ImageTexture* ImageTexture::LoadTextureMap(const std::string& path)
{
	if (m_LoadedTextures.find(path) == m_LoadedTextures.end())
	{
		int width, height, bpp;
		stbi_set_flip_vertically_on_load(false);
		GLubyte* data = stbi_load(path.c_str(), &width, &height, &bpp, 0);

		if (data)
		{
			ImageTexture* texture = new ImageTexture(GL_TEXTURE_CUBE_MAP, path);
			glGenTextures(1, &texture->m_TextureId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture->m_TextureId);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

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

				int internalFormat = bpp == 3 ? GL_RGB : GL_RGBA;
				int format = bpp == 3 ? GL_RGB : GL_RGBA;
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + side, 0, internalFormat, chunkWidth, chunkHeight, 0, format, GL_UNSIGNED_BYTE, chunkData.data());
			}

			stbi_image_free(data);
			return m_LoadedTextures[path] = texture;
		}
		else
		{
			stbi_image_free(data);
			std::runtime_error("[ImageTexture::LoadImageMap] : Couldn't load image >> " + path);
		}
	}
	
	return m_LoadedTextures.at(path);
}

std::string ImageTexture::CombinePaths(const std::vector<std::string>& paths)
{
	std::string combinePath;
	
	for (auto& path : paths)
	{
		combinePath += path;
	}

	return combinePath;
}