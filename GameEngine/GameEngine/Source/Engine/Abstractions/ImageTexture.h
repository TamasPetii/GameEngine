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

	static ImageTexture* LoadImage(const std::string& path);
	static ImageTexture* LoadImageMap(const std::string& path);
	static ImageTexture* LoadImageMap(const std::vector<std::string>& paths);

	//Getter
	auto Get_Path() const { return m_Path; }
	auto Get_TextureId() const { return m_TextureId; }
	auto Get_TextureType() const { return m_TextureType; }
private:
	std::string m_Path;
	GLuint m_TextureId;
	GLuint m_TextureType;
	static std::unordered_map<std::string, ImageTexture*> m_LoadedTextures;
	static std::string CombinePaths(const std::vector<std::string>& paths);
};
