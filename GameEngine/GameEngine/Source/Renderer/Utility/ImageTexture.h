#pragma once
#include <STB_IMAGE/stb_image.h>
#include <GLEW/glew.h>
#include <unordered_map>
#include <iostream>
#include <string>

class ImageTexture
{
public:
	ImageTexture(GLuint type, const std::string& path) : m_TextureType(type), m_Path(path) {}
	~ImageTexture();

	static ImageTexture* LoadTexture(const std::string& path);
	static ImageTexture* LoadTextureMap(const std::string& path);
	static ImageTexture* LoadTextureMap(const std::vector<std::string>& paths);

	/*Getter*/
	inline const auto& Get_Path() const { return m_Path; }
	inline const auto& Get_TextureId() const { return m_TextureId; }
	inline const auto& Get_TextureType() const { return m_TextureType; }
private:
	std::string m_Path;
	GLuint m_TextureId;
	GLuint m_TextureType;
	static std::unordered_map<std::string, ImageTexture*> m_LoadedTextures;
	static std::string CombinePaths(const std::vector<std::string>& paths);
};
