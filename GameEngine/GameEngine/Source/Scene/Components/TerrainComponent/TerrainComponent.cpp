#include "TerrainComponent.h"

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

void TerrainComponent::GenerateFromPerlinNoise()
{

}