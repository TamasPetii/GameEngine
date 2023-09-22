#include "Grid.h"

Grid* Grid::Clone() const
{
    return new Grid(*this);
}

Grid::Grid(const Grid& other)
    : Shape(other)
{
    /*
    m_Name = other.m_Name;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GenerateShape(vertices, indices);
    UploadToGpu(vertices, indices);
    */
}

Grid::Grid()
{
    m_Name = "Grid";
    m_Height = 50;
    m_Width = 50;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GenerateShape(vertices, indices);
    UploadToGpu(vertices, indices);
}

Grid::Grid(const std::vector<std::vector<float>>& heightMap)
{
    m_Height = heightMap.size();
    m_Width = heightMap[0].size();
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GenerateShape(vertices, indices, heightMap);
    UploadToGpu(vertices, indices);
}

void Grid::GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
    for (unsigned int h = 0; h < m_Height; ++h)
    {
        for (unsigned int w = 0; w < m_Width; ++w)
        {
            float u = w / static_cast<float>(m_Width);
            float v = h / static_cast<float>(m_Height);

            glm::vec3 position = glm::vec3(w, 0, h);
            glm::vec2 texture = glm::vec2(u, 1 - v);
            vertices.push_back(Vertex(position, texture));
        }

    }

    for (unsigned int h = 0; h < m_Height - 1; h++)
    {
        for (unsigned int w = 0; w < m_Width - 1; w++)
        {
            int c = w + h * m_Width;
            indices.push_back(c);
            indices.push_back(c + m_Width);
            indices.push_back(c + m_Width + 1);
            indices.push_back(c);
            indices.push_back(c + m_Width + 1);
            indices.push_back(c + 1);
        }
    }
}

void Grid::GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, const std::vector<std::vector<float>>& heightMap)
{
    for (unsigned int h = 0; h < m_Height; ++h)
    {
        for (unsigned int w = 0; w < m_Width; ++w)
        {
            float u = w / static_cast<float>(m_Width);
            float v = h / static_cast<float>(m_Height);

            glm::vec3 position = glm::vec3(w, heightMap[h][w], h);
            glm::vec2 texture = glm::vec2(u, 1 - v);
            vertices.push_back(Vertex(position, texture));
        }

    }

    for (unsigned int h = 0; h < m_Height - 1; h++)
    {
        for (unsigned int w = 0; w < m_Width - 1; w++)
        {
            int c = w + h * m_Width;
            indices.push_back(c);
            indices.push_back(c + m_Width);
            indices.push_back(c + m_Width + 1);

            GenerateSmoothTBN(vertices[c], vertices[c + m_Width], vertices[c + m_Width + 1]);

            indices.push_back(c);
            indices.push_back(c + m_Width + 1);
            indices.push_back(c + 1);

            GenerateSmoothTBN(vertices[c], vertices[c + m_Width + 1], vertices[c + 1]);
        }
    }
}