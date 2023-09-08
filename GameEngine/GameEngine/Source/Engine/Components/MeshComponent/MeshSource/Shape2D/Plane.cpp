#include "Plane.h"

Plane::Plane()
{
    m_Name = "Plane";

    std::vector<Vertex> vertices = std::vector<Vertex>
    {
        //Top
        Vertex(glm::vec3(-1, 0, -1), glm::vec3(0,1,0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, 0, 1), glm::vec3(0,1,0), glm::vec2(0, 0)),
        Vertex(glm::vec3(1, 0, 1), glm::vec3(0,1,0), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, 0, -1), glm::vec3(0,1,0), glm::vec2(1, 1))
    };

    std::vector<GLuint> indices;

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(3);

    auto TB = Shape::GenerateTB(vertices[0].normal);

    for (int j = 0; j < 4; j++)
    {
        vertices[j].tangent = TB.first;
        vertices[j].bitangent = TB.second;
    }

    UploadToGpu(vertices, indices);
}
