#pragma once
#include <GLM/glm.hpp>
#include <vector>
#include <random>

class PerlinNoise
{
public:
	PerlinNoise();
	float ReadNoiseValue(float x, float y);

	static float lerp(float a, float b, float t);
	static float fade(float t);
private:
	const unsigned int size = 100;
	std::vector<std::vector<glm::vec2>> m_Seeds;
	std::vector<std::vector<glm::vec2>> m_Gradients;
};

inline float PerlinNoise::lerp(float a, float b, float t)
{
	return (1 - t) * a + t * b;
}

inline float PerlinNoise::fade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}
