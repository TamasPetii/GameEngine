#include "Cube.h"

Cube::Cube()
{
    std::cout << "Generated Cube" << std::endl;

    m_Name = "Cube";

    std::vector<Vertex> vertices = std::vector<Vertex>
    {
        //Front
        Vertex(glm::vec3(-1, 1, 1), glm::vec3(0,0,1), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec3(0,0,1), glm::vec2(0, 0)),
        Vertex(glm::vec3(1, -1, 1), glm::vec3(0,0,1), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, 1, 1), glm::vec3(0,0,1), glm::vec2(1, 1)),

        //Back
        Vertex(glm::vec3(1, 1, -1), glm::vec3(0,0,-1), glm::vec2(0, 1)),
        Vertex(glm::vec3(1, -1, -1), glm::vec3(0,0,-1),glm::vec2(0, 0)),
        Vertex(glm::vec3(-1, -1, -1), glm::vec3(0,0,-1),glm::vec2(1, 0)),
        Vertex(glm::vec3(-1, 1, -1), glm::vec3(0,0,-1),glm::vec2(1, 1)),

        //Left
        Vertex(glm::vec3(-1, 1, -1), glm::vec3(-1,0,0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, -1, -1), glm::vec3(-1,0,0),glm::vec2(0, 0)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec3(-1,0,0),glm::vec2(1, 0)),
        Vertex(glm::vec3(-1, 1, 1), glm::vec3(-1,0,0),glm::vec2(1, 1)),

        //Right
        Vertex(glm::vec3(1, 1, 1), glm::vec3(1,0,0), glm::vec2(0, 1)),
        Vertex(glm::vec3(1, -1, 1), glm::vec3(1,0,0), glm::vec2(0, 0)),
        Vertex(glm::vec3(1, -1, -1), glm::vec3(1,0,0), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, 1, -1), glm::vec3(1,0,0), glm::vec2(1, 1)),

        //Top
        Vertex(glm::vec3(-1, 1, -1), glm::vec3(0,1,0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, 1, 1), glm::vec3(0,1,0), glm::vec2(0, 0)),
        Vertex(glm::vec3(1, 1, 1), glm::vec3(0,1,0), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, 1, -1), glm::vec3(0,1,0), glm::vec2(1, 1)),

        //Bottom
        Vertex(glm::vec3(-1, -1, 1), glm::vec3(0,-1,0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, -1, -1), glm::vec3(0,-1,0), glm::vec2(0, 0)),
        Vertex(glm::vec3(1, -1, -1), glm::vec3(0,-1,0), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, -1, 1), glm::vec3(0,-1,0), glm::vec2(1, 1))
    };

    std::vector<GLuint> indices;
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

    UploadToGpu(vertices, indices);
}
