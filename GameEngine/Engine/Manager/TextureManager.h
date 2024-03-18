#pragma once
#include "EngineApi.h"
#include <string>
#include <unordered_map>
#include <Render/OpenGL/TextureGL.h>
#include <stb_image.h>

class ENGINE_API TextureManager
{
public:
	static TextureManager* Instance();
	static void Destroy();
	const auto& GetAllTexture() { return m_Textures; }
	bool IsImageTextureLoaded(const std::string& path) { return m_Textures.find(path) != m_Textures.end(); }
	std::shared_ptr<TextureGL> GetImageTexture(const std::string& path) { return m_Textures[path]; }
	std::shared_ptr<TextureGL> LoadImageTexture(const std::string& path);
	std::shared_ptr<TextureGL> LoadImageTextureMap(const std::string& path);
private:
	TextureManager() = default;
	static TextureManager* m_Instance;
	std::unordered_map<std::string, std::shared_ptr<TextureGL>> m_Textures;
};

