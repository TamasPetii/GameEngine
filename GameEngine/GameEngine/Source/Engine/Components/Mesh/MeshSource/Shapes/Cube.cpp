#include "Cube.h"

Shape<Cube>::Shape() : MeshBase()
{
    name = "Cube";
    LoadVertices();
    CreateNormals();
    LoadIndices();
    UploadBuffersToGpu();
}

void Shape<Cube>::LoadVertices()
{
	mVertices.clear();
	mVertices = std::vector<Vertex>
	{
        Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 0)),
        Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 1)),

        Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1)),
        Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0)),
        Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0)),
        Vertex(glm::vec3(-1, 1, -1), glm::vec2(1, 1)),

        Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 0)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0)),
        Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1)),

        Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1)),
        Vertex(glm::vec3(1, -1, 1), glm::vec2(0, 0)),
        Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, 1, -1), glm::vec2(1, 1)),

        Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0)),
        Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, 1, -1), glm::vec2(1, 1)),

        Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 0)),
        Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1))
	};
}

void Shape<Cube>::CreateNormals()
{
    for (unsigned int i = 0; i < 24; i += 4)
    {
        glm::vec3 normal = MeshBase::GenerateNormalVectors(mVertices[0 + i], mVertices[1 + i], mVertices[2 + i]);
        mVertices[0 + i].mNormal = normal;
        mVertices[1 + i].mNormal = normal;
        mVertices[2 + i].mNormal = normal;
        mVertices[0 + i].mNormal = normal;
        mVertices[2 + i].mNormal = normal;
        mVertices[3 + i].mNormal = normal;
    }
}

void Shape<Cube>::LoadIndices()
{
    mIndices.clear();
    for (unsigned int i = 0; i < 24; i += 4)
    {
        mIndices.push_back(0 + i);
        mIndices.push_back(1 + i);
        mIndices.push_back(2 + i);
        mIndices.push_back(0 + i);
        mIndices.push_back(2 + i);
        mIndices.push_back(3 + i);
    }
}
