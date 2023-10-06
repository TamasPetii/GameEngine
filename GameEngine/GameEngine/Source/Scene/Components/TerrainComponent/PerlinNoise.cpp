#include "PerlinNoise.h"

PerlinNoise::PerlinNoise()
{
	m_Seeds = std::vector<std::vector<glm::vec2>>(size, std::vector<glm::vec2>(size));
	m_Gradients = std::vector<std::vector<glm::vec2>>(size, std::vector<glm::vec2>(size));

	static std::random_device rnd;
	static std::mt19937 gen(rnd());
	static std::uniform_int_distribution<int> distInt;
	static std::uniform_real_distribution<float> distFloat(-1.0f, 1.0f);

	for (unsigned int h = 0; h < size; ++h)
	{
		for (unsigned int w = 0; w < size; ++w)
		{
			m_Seeds[h][w].x = distInt(gen);
			m_Seeds[h][w].y = distInt(gen);
		}
	}

	for (unsigned int h = 0; h < size; ++h)
	{
		for (unsigned int w = 0; w < size; ++w)
		{
			int currentSeedX = w;
			int currentSeedY = h;
			std::mt19937 gen1(m_Seeds[currentSeedY][currentSeedX].x);
			std::mt19937 gen2(m_Seeds[currentSeedY][currentSeedX].y);
			m_Gradients[h][w].x = distFloat(gen1);
			m_Gradients[h][w].y = distFloat(gen2);
			glm::normalize(m_Gradients[h][w]);
		}
	}
}

float PerlinNoise::ReadNoiseValue(float x, float y)
{
	if (x < 0 || x >= size - 2 || y < 0 || y >= size - 2)
	{
		float remainderX = x - int(x);
		float remainderY = y - int(y);
		x = int(x) % (size - 2) + remainderX;
		y = int(y) % (size - 2) + remainderY;
	}

	// 1----2
	// |    |
	// |  x |
	// 3----4

	//This is point x
	float hitW = x;
	float hitH = y;

	//They will be at point 1.
	int startH = static_cast<int>(hitH);
	int startW = static_cast<int>(hitW);

	glm::vec2 cornerToHitVector;
	glm::vec2 gradientVector;

	//1. Left Top
	cornerToHitVector = glm::vec2(hitH, hitW) - glm::vec2(startH, startW);
	gradientVector = m_Gradients[startH][startW];
	float height1 = glm::dot(gradientVector, cornerToHitVector);

	//2. Right Top
	cornerToHitVector = glm::vec2(hitH, hitW) - glm::vec2(startH, startW + 1);
	gradientVector = m_Gradients[startH][startW + 1];
	float height2 = glm::dot(gradientVector, cornerToHitVector);

	//3. Left Bottom
	cornerToHitVector = glm::vec2(hitH, hitW) - glm::vec2(startH + 1, startW);
	gradientVector = m_Gradients[startH + 1][startW];
	float height3 = glm::dot(gradientVector, cornerToHitVector);

	//4. Right Bottom
	cornerToHitVector = glm::vec2(hitH, hitW) - glm::vec2(startH + 1, startW + 1);
	gradientVector = m_Gradients[startH + 1][startW + 1];
	float height4 = glm::dot(gradientVector, cornerToHitVector);

	float interpolateHeight12 = lerp(height1, height2, fade(hitW - static_cast<float>(startW)));
	float interpolateHeight34 = lerp(height3, height4, fade(hitW - static_cast<float>(startW)));
	float interpolateHeight1234 = lerp(interpolateHeight12, interpolateHeight34, fade(hitH - static_cast<float>(startH)));

	return interpolateHeight1234;
}