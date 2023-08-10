#include "Texture.h"

std::unordered_map<std::string, Texture2D*> Texture2D::mLoadedTextures;

Texture::Texture()
{
	glGenTextures(1, &mTextureId);
}

Texture::~Texture()
{
	glDeleteTextures(1, &mTextureId);
}

Texture2D::Texture2D(const std::string& path)
	: Texture()
{
	glBindTexture(GL_TEXTURE_2D, mTextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	GLint width, height, bpp;
	stbi_set_flip_vertically_on_load(true);
	GLubyte* data = stbi_load(path.c_str(), &width, &height, &bpp, 4);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else
	{
		stbi_image_free(data);
		throw std::runtime_error("Texture Creation Error");
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	std::cout << "Loaded texture: " << path << std::endl;
}

Texture2D* Texture2D::LoadTexture2D(const std::string& path)
{
	//Texture has not been loaded
	if (mLoadedTextures.find(path) == mLoadedTextures.end())
	{
		mLoadedTextures[path] = new Texture2D(path);
	}
	return mLoadedTextures[path];
}

Texture2D* Texture2D::GetTexture2D(const std::string& path)
{
	return mLoadedTextures[path];
}

std::vector<std::string> Texture2D::GetTextureNames()
{
	std::vector<std::string> textureNames;

	for (auto& texture : mLoadedTextures)
	{
		//textureNames.push_back(std::filesystem::path(texture.first).filename().string());
		textureNames.push_back(texture.first);
	}

	return textureNames;
}

void Texture2D::ClearTextures()
{
	for (auto element : mLoadedTextures)
	{
		delete element.second;
	}
}