#pragma once
#include "../Component.h"
#include "PerlinNoise.h"
#include <GLEW/glew.h>
#include <STB_IMAGE/stb_image.h>
#include <iostream>
#include <string>

enum class TerrainType
{
	HEIGHT_MAP,
	PERLIN_NOISE
};

class TerrainComponent : public Component
{
public:
	TerrainComponent(unsigned int width, unsigned int height, unsigned int scale);
	
	json SaveToJson() const override { return json(); }
	void LoadFromJson(const json& object) override { return; }
	TerrainComponent* Clone() const override { return nullptr; }

	void GenerateFromTexture(const std::string& path);
	void GenerateFromPerlinNoise(unsigned int depth);
	
	inline const auto& Get_Width() const { return m_Width; }
	inline const auto& Get_Height() const { return m_Height; }
	inline const auto& Get_TerrainMap() const { return m_TerrainMap; }
	inline const auto& Get_NoiseTexture() const { return m_NoiseTexture; }

	float ReadHeight(int h, int w);
	float ReadHeightInterpolated(float x, float y);
private:
	unsigned int m_Width;
	unsigned int m_Height;
	unsigned int m_Scale;
	unsigned int m_NoiseTexture;
	std::vector<std::vector<float>> m_TerrainMap;
};

