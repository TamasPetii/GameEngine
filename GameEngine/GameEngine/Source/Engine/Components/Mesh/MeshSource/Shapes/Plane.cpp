#include "Plane.h"

Shape<Plane>::Shape() : MeshBase()
{
    LoadVertices();
    LoadIndices();
    UploadBuffersToGpu();
}

void Shape<Plane>::LoadVertices()
{
    mVertices.clear();
    mVertices.push_back(Vertex(glm::vec3(-1, 0, -1), glm::vec3(0, 1, 0), glm::vec2(0, 1)));
    mVertices.push_back(Vertex(glm::vec3(-1, 0, 1), glm::vec3(0, 1, 0), glm::vec2(0, 0)));
    mVertices.push_back(Vertex(glm::vec3(1, 0, 1), glm::vec3(0, 1, 0), glm::vec2(1, 0)));
    mVertices.push_back(Vertex(glm::vec3(1, 0, -1), glm::vec3(0, 1, 0), glm::vec2(1, 1)));
}

void Shape<Plane>::LoadIndices()
{
    mIndices.clear();
    mIndices.push_back(0);
    mIndices.push_back(1);
    mIndices.push_back(2);
    mIndices.push_back(0);
    mIndices.push_back(2);
    mIndices.push_back(3);
}
