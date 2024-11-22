#pragma once
#include "EngineApi.h"
#include <unordered_map>
#include <stb_image.h>
#include <string>
#include <algorithm>
#include <filesystem>


#include "Logger/Logger.h"
#include "Render/OpenGL/TextureGL.h"
#include "Settings/GlobalSettings.h"

class ENGINE_API TextureManager
{
public:
	static TextureManager* Instance();
	static void Destroy();
	const auto& GetAllTexture() { return m_Textures; }
	const auto& GetAllTextureMaps() { return m_TexturesMaps; }
	bool IsImageTextureLoaded(const std::string& path) { return m_Textures.find(path) != m_Textures.end(); }
	bool IsImageTextureMapLoaded(const std::string& path) { return m_TexturesMaps.find(path) != m_TexturesMaps.end(); }
	std::shared_ptr<TextureGL> GetImageTexture(const std::string& path) { return m_Textures[path]; }
	std::shared_ptr<TextureGL> GetImageTextureMap(const std::string& path) { return m_TexturesMaps[path]; }
	std::shared_ptr<TextureGL> LoadImageTexture(const std::string& path);
	std::shared_ptr<TextureGL> LoadImageTextureMap(const std::string& path);

	auto& RefAllTextues() { return m_Textures; }
private:
	TextureManager() = default;
	static TextureManager* m_Instance;
	std::unordered_map<std::string, std::shared_ptr<TextureGL>> m_Textures;
	std::unordered_map<std::string, std::shared_ptr<TextureGL>> m_TexturesMaps;
};

