#include "TerrainComponent.h"

TerrainComponent::TerrainComponent(unsigned int width, unsigned int height, unsigned int scale)
{
	m_Width = width;
	m_Height = height;
	m_Scale = scale;

	m_TerrainMap = std::vector<std::vector<float>>(m_Height, std::vector<float>(m_Width));
}

void TerrainComponent::GenerateFromTexture(const std::string& path)
{
	int width, height, bpp;
	stbi_set_flip_vertically_on_load(true);
	GLubyte* data = stbi_load(path.c_str(), &width, &height, &bpp, 0);

	m_Width = width;
	m_Height = height;

	if (data)
	{
		m_TerrainMap.resize(m_Height);

		for (unsigned int h = 0; h < m_Height; ++h)
		{
			m_TerrainMap[h].resize(m_Width);

			for (unsigned int w = 0; w < m_Width; ++w)
			{
				unsigned char* pixel = data + (w + h * m_Width) * bpp;
				m_TerrainMap[h][w] = static_cast<float>(*pixel);
			}
		}
	}
	else
	{
		std::cout << "Invalid terrain texture!" << std::endl;
	}

	stbi_image_free(data);
}

void TerrainComponent::GenerateFromPerlinNoise(unsigned int depth)
{
	static PerlinNoise noise;

	for (int h = 0; h < m_Height; h++)
	{
		for (int w = 0; w < m_Width; w++)
		{
			float amplitude = 1.0f;
			float frequency = 1.0f;
			float noiseValue = 0.0f;

			for (int i = 0; i < depth; i++)
			{
				float sampleX = (w / static_cast<float>(m_Width)) * m_Scale * frequency;
				float sampleY = (h / static_cast<float>(m_Height)) * m_Scale * frequency;
				float noiseSample = noise.ReadNoiseValue(sampleX, sampleY);

				noiseValue += amplitude * noiseSample;

				amplitude /= 2.f;
				frequency *= 2.0;
			}

			m_TerrainMap[h][w] = (noiseValue + 1) / 2;
		}
	}

	if (m_NoiseTexture != 0)
		glDeleteTextures(1, &m_NoiseTexture);

	glGenTextures(1, &m_NoiseTexture);
	glBindTexture(GL_TEXTURE_2D, m_NoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, m_Width, m_Height, 0, GL_RED, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	for (unsigned int i = 0; i < m_Height; ++i)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, m_Height - (i + 1), m_Width, 1, GL_RED, GL_FLOAT, m_TerrainMap[m_Height - (i + 1)].data());
	}
}

float TerrainComponent::ReadHeight(int h, int w)
{
	return m_TerrainMap[h][w];
}

float TerrainComponent::ReadHeightInterpolated(float x, float y)
{
	int startX = static_cast<int>(x);
	int startY = static_cast<int>(y);

	float height1 = m_TerrainMap[startY][startX];
	float height2 = m_TerrainMap[startY][startX + 1];
	float height3 = m_TerrainMap[startY + 1][startX];
	float height4 = m_TerrainMap[startY + 1][startX + 1];

	float interpolateHeight12 = PerlinNoise::lerp(height1, height2, x - startX);
	float interpolateHeight34 = PerlinNoise::lerp(height3, height4, x - startX);
	float interpolateHeight1234 = PerlinNoise::lerp(interpolateHeight12, interpolateHeight34, y - startY);

	return interpolateHeight1234;
}