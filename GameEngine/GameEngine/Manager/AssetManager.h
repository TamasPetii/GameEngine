#pragma once
#include <string>
#include <unordered_map>
#include <Render/OpenGL/TextureGL.h>
#include <stb_image.h>

class AssetManager
{
public:
	auto GetImageTexture(const std::string& path) { return m_Textures[path]; }
	std::shared_ptr<TextureGL> LoadImageTexture(const std::string& path);
	std::shared_ptr<TextureGL> LoadImageTextureMap(const std::string& path);
private:
	std::unordered_map<std::string, std::shared_ptr<TextureGL>> m_Textures;
};

