#include "Shape.h"

std::unordered_set<Shape*> Shape::m_ShapeInstances;

std::tuple<glm::vec3, glm::vec3, glm::vec3> Shape::GenerateTBN(const std::tuple<glm::vec3, glm::vec3, glm::vec3>& positions, const std::tuple<glm::vec2, glm::vec2, glm::vec2>& textures)
{
    glm::vec3 edge1 = std::get<1>(positions) - std::get<0>(positions);
    glm::vec3 edge2 = std::get<2>(positions) - std::get<0>(positions);

    glm::vec2 deltaUV1 = std::get<1>(textures) - std::get<0>(textures);
    glm::vec2 deltaUV2 = std::get<2>(textures) - std::get<0>(textures);

    float det = 1.0 / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    glm::vec3 tangent;
    tangent.x = det * (edge1.x * deltaUV2.y - edge2.x * deltaUV1.y);
    tangent.y = det * (edge1.y * deltaUV2.y - edge2.y * deltaUV1.y);
    tangent.z = det * (edge1.z * deltaUV2.y - edge2.z * deltaUV1.y);

    glm::vec3 bitangent;
    bitangent.x = det * (-edge1.x * deltaUV2.x + edge2.x * deltaUV1.x);
    bitangent.y = det * (-edge1.y * deltaUV2.x + edge2.y * deltaUV1.x);
    bitangent.z = det * (-edge1.z * deltaUV2.x + edge2.z * deltaUV1.x);

    glm::vec3 normal = glm::cross(edge1, edge2);

    return std::make_tuple(glm::normalize(tangent), glm::normalize(bitangent), glm::normalize(normal));
}

std::pair<glm::vec3, glm::vec3> Shape::GenerateTB(const glm::vec3& normal)
{
    glm::vec3 up = glm::vec3(0, 1, 0);
    if (glm::dot(up, normal) > 0.99999f)
    {
        up = glm::vec3(1, 0, 0); // Choose another up vector
    }

    glm::vec3 tangent = glm::normalize(glm::cross(up, normal));
    glm::vec3 bitangent = glm::normalize(glm::cross(normal, tangent));
    return { tangent, bitangent };
}