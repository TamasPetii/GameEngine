#include "Cube.h"

Cube* Cube::Clone() const
{
    return new Cube(*this);
}

Cube::Cube(const Cube& other)
    : Shape(other)
{
    m_Name = other.m_Name;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GenerateShape(vertices, indices);
    UploadToGpu(vertices, indices);
}

Cube::Cube()
{
    m_Name = "Cube";
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GenerateShape(vertices, indices);
    UploadToGpu(vertices, indices);
}

void Cube::GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{

        //Front
    vertices.push_back(Vertex(glm::vec3(-1, 1, 1), glm::vec3(0, 0, 1), glm::vec2(0, 1)));
    vertices.push_back(Vertex(glm::vec3(-1, -1, 1), glm::vec3(0,0,1), glm::vec2(0, 0)));
    vertices.push_back(Vertex(glm::vec3(1, -1, 1), glm::vec3(0,0,1), glm::vec2(1, 0)));
    vertices.push_back(Vertex(glm::vec3(1, 1, 1), glm::vec3(0,0,1), glm::vec2(1, 1)));

        //Back
    vertices.push_back(Vertex(glm::vec3(1, 1, -1), glm::vec3(0,0,-1), glm::vec2(0, 1)));
    vertices.push_back(Vertex(glm::vec3(1, -1, -1), glm::vec3(0,0,-1),glm::vec2(0, 0)));
    vertices.push_back(Vertex(glm::vec3(-1, -1, -1), glm::vec3(0,0,-1),glm::vec2(1, 0)));
    vertices.push_back(Vertex(glm::vec3(-1, 1, -1), glm::vec3(0,0,-1),glm::vec2(1, 1)));

        //Left
    vertices.push_back(Vertex(glm::vec3(-1, 1, -1), glm::vec3(-1,0,0), glm::vec2(0, 1)));
    vertices.push_back(Vertex(glm::vec3(-1, -1, -1), glm::vec3(-1,0,0),glm::vec2(0, 0)));
    vertices.push_back(Vertex(glm::vec3(-1, -1, 1), glm::vec3(-1,0,0),glm::vec2(1, 0)));
    vertices.push_back(Vertex(glm::vec3(-1, 1, 1), glm::vec3(-1,0,0),glm::vec2(1, 1)));

        //Right
    vertices.push_back(Vertex(glm::vec3(1, 1, 1), glm::vec3(1,0,0), glm::vec2(0, 1)));
    vertices.push_back(Vertex(glm::vec3(1, -1, 1), glm::vec3(1,0,0), glm::vec2(0, 0)));
    vertices.push_back(Vertex(glm::vec3(1, -1, -1), glm::vec3(1,0,0), glm::vec2(1, 0)));
    vertices.push_back(Vertex(glm::vec3(1, 1, -1), glm::vec3(1,0,0), glm::vec2(1, 1)));

        //Top
    vertices.push_back(Vertex(glm::vec3(-1, 1, -1), glm::vec3(0,1,0), glm::vec2(0, 1)));
    vertices.push_back(Vertex(glm::vec3(-1, 1, 1), glm::vec3(0,1,0), glm::vec2(0, 0)));
    vertices.push_back(Vertex(glm::vec3(1, 1, 1), glm::vec3(0,1,0), glm::vec2(1, 0)));
    vertices.push_back(Vertex(glm::vec3(1, 1, -1), glm::vec3(0,1,0), glm::vec2(1, 1)));

        //Bottom
    vertices.push_back(Vertex(glm::vec3(-1, -1, 1), glm::vec3(0,-1,0), glm::vec2(0, 1)));
    vertices.push_back(Vertex(glm::vec3(-1, -1, -1), glm::vec3(0,-1,0), glm::vec2(0, 0)));
    vertices.push_back(Vertex(glm::vec3(1, -1, -1), glm::vec3(0,-1,0), glm::vec2(1, 0)));
    vertices.push_back(Vertex(glm::vec3(1, -1, 1), glm::vec3(0,-1,0), glm::vec2(1, 1)));

    for (unsigned int i = 0; i < 24; i += 4)
    {
        indices.push_back(0 + i);
        indices.push_back(1 + i);
        indices.push_back(2 + i);
        indices.push_back(0 + i);
        indices.push_back(2 + i);
        indices.push_back(3 + i);

        auto TB = Shape::GenerateTB(vertices[0 + i].normal);

        for (int j = 0; j < 4; j++)
        {
            vertices[j + i].tangent = TB.first;
            vertices[j + i].bitangent = TB.second;
        }
    }
}