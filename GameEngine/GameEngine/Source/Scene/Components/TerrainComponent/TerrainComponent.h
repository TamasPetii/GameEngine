#pragma once
#include "../Component.h"
#include <GLEW/glew.h>
#include <STB_IMAGE/stb_image.h>
#include <iostream>
#include <string>

enum class TerrainType
{
	HEIGHT_MAP,
	PERLIN_NOISE,

};

class TerrainComponent : public Component
{
public:
	json SaveToJson() const override { return json(); }
	void LoadFromJson(const json& object) override { return; }
	TerrainComponent* Clone() const override { return nullptr; }

	void GenerateFromTexture(const std::string& path);
	void GenerateFromPerlinNoise();
	
	inline const auto& Get_Width() const { return m_Width; }
	inline const auto& Get_Height() const { return m_Height; }
	inline const auto& Get_TerrainMap() const { return m_TerrainMap; }
private:
	unsigned int m_Width;
	unsigned int m_Height;
	std::vector<std::vector<float>> m_TerrainMap;
};

