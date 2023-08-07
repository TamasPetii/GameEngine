#pragma once
#include <GLEW/glew.h>
#include <STB_IMAGE/stb_image.h>
#include <iostream>
#include <string>
#include <unordered_map>

class Texture
{
public:
	Texture();
	~Texture();
	inline virtual int GetType() const = 0;
	inline unsigned int GetTextureId() const { return mTextureId; }
protected:
	unsigned int mTextureId;
};

class Texture2D : public Texture
{
public:
	inline int GetType() const override { return GL_TEXTURE_2D; }
	static Texture2D* LoadTexture2D(const std::string& path);
	static void ClearTextures();
private:
	Texture2D(const std::string& path);
	static std::unordered_map<std::string, Texture2D*> mLoadedTextures;
};

/*
class TextureMap : public Texture
{
public:
	inline int GetType() const override { return GL_TEXTURE_CUBE_MAP; }
	static TextureMap* LoadTextureMap(const std::string& path);
	static void ClearTextures();
private:
	TextureMap(const std::string& path);
	static std::unordered_map<std::string, TextureMap*> mLoadedTextures;
};
*/
