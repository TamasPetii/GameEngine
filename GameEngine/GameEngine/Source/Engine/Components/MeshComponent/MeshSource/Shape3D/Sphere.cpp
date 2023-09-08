#include "Sphere.h"

Sphere::Sphere()
{
    m_Name = "Sphere";
    m_Radius = 1.f;
    m_Count = 50;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GenerateShape(vertices, indices);

    UploadToGpu(vertices, indices);
}

void Sphere::GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
    for (unsigned int h = 0; h <= m_Count; h++)
    {
        for (unsigned int w = 0; w <= m_Count; w++)
        {
            float u = w / static_cast<float>(m_Count);
            float v = h / static_cast<float>(m_Count);
            float theta = 2 * M_PI * u;
            float phi = M_PI * (0.5f - v);

            float x = m_Radius * cosf(phi) * sinf(theta);
            float y = m_Radius * sinf(phi);
            float z = m_Radius * cosf(phi) * cosf(theta);

            auto normal = glm::vec3(x, y, z);
            auto TB = Shape::GenerateTB(normal);

            Vertex vertex;
            vertex.position = glm::vec3(x, y, z);
            vertex.normal = glm::normalize(normal);
            vertex.tangent = glm::normalize(TB.first);
            vertex.bitangent = glm::normalize(TB.second);
            vertex.texture = glm::vec2(1 - u, 1 - v);
            vertices.push_back(vertex);
        }
    }

    for (unsigned int h = 0; h < m_Count; h++)
    {
        for (unsigned int w = 0; w < m_Count; w++)
        {
            int c = w + h * (m_Count + 1);
            indices.push_back(c);
            indices.push_back(c + m_Count + 1);
            indices.push_back(c + m_Count + 2);
            indices.push_back(c);
            indices.push_back(c + m_Count + 2);
            indices.push_back(c + 1);
        }
    }
}

void Sphere::ReGenerate()
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GenerateShape(vertices, indices);

    m_VertexCount = vertices.size();
    m_IndexCount = indices.size();
    m_Vbo->AttachData(vertices, GL_STATIC_DRAW);
    m_Ibo->AttachData(indices, GL_STATIC_DRAW);
}

/*
Sphere::Sphere()
{
    m_Radius = 1.f;
    m_Count = 25;

    std::vector<Vertex> vertices;
    for (unsigned int h = 0; h <= m_Count; h++)
    {
        for (unsigned int w = 0; w <= m_Count; w++)
        {
            float u = w / static_cast<float>(m_Count);
            float v = h / static_cast<float>(m_Count);
            float theta = 2 * M_PI * u;
            float phi = M_PI * (0.5f - v);

            float x = m_Radius * cosf(phi) * sinf(theta);
            float y = m_Radius * sinf(phi);
            float z = m_Radius * cosf(phi) * cosf(theta);

            Vertex vertex;
            vertex.position = glm::vec3(x, y, z);
            vertex.normal = glm::normalize(glm::vec3(x, y, z));
            vertex.texture = glm::vec2(1 - u, 1 - v);
            vertices.push_back(vertex);
        }
    }

    std::vector<GLuint> indices;
    for (unsigned int h = 0; h < m_Count; h++)
    {
        for (unsigned int w = 0; w < m_Count; w++)
        {
            int c = w + h * (m_Count + 1);
            indices.push_back(c);
            indices.push_back(c + m_Count + 1);
            indices.push_back(c + m_Count + 2);
            indices.push_back(c);
            indices.push_back(c + m_Count + 2);
            indices.push_back(c + 1);

            auto pos0 = std::make_tuple(vertices[c].position, vertices[c + m_Count + 1].position, vertices[c + m_Count + 2].position);
            auto tex0 = std::make_tuple(vertices[c].texture, vertices[c + m_Count + 1].texture, vertices[c + m_Count + 2].texture);
            auto tbn0 = Shape::GenerateTBN(pos0, tex0);

            vertices[c].tangent += std::get<0>(tbn0);
            vertices[c].bitangent += std::get<1>(tbn0);

            vertices[c + m_Count + 1].tangent += std::get<0>(tbn0);
            vertices[c + m_Count + 1].bitangent += std::get<1>(tbn0);

            vertices[c + m_Count + 2].tangent += std::get<0>(tbn0);
            vertices[c + m_Count + 2].bitangent += std::get<1>(tbn0);

            auto pos1 = std::make_tuple(vertices[c].position, vertices[c + m_Count + 2].position, vertices[c + 1].position);
            auto tex1 = std::make_tuple(vertices[c].texture, vertices[c + m_Count + 2].texture, vertices[c + 1].texture);
            auto tbn1 = Shape::GenerateTBN(pos1, tex1);

            vertices[c].tangent += std::get<0>(tbn1);
            vertices[c].bitangent += std::get<1>(tbn1);

            vertices[c + m_Count + 2].tangent += std::get<0>(tbn1);
            vertices[c + m_Count + 2].bitangent += std::get<1>(tbn1);

            vertices[c + 1].tangent += std::get<0>(tbn1);
            vertices[c + 1].bitangent += std::get<1>(tbn1);

        }
    }

    UploadToGpu(vertices, indices);
}
*/

/*
Sphere::Sphere()
{
    m_Radius = 1.f;
    m_Count = 25;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textures;
    for (unsigned int h = 0; h <= m_Count; h++)
    {
        for (unsigned int w = 0; w <= m_Count; w++)
        {
            float u = w / static_cast<float>(m_Count);
            float v = h / static_cast<float>(m_Count);
            float theta = 2 * M_PI * u;
            float phi = M_PI * (0.5f - v);

            float x = m_Radius * cosf(phi) * sinf(theta);
            float y = m_Radius * sinf(phi);
            float z = m_Radius * cosf(phi) * cosf(theta);

            //float x = m_Radius * sinf(theta);
            //float y = m_Radius * (1 - v);
            //float z = m_Radius * cosf(theta);

            positions.push_back(glm::vec3(x, y, z));
            normals.push_back(glm::vec3(x, y, z));
            textures.push_back(glm::vec2(1 - u, 1 - v));
        }
    }

    struct Vec3Hash
    {
        std::size_t operator()(const glm::vec3& vec) const
        {
            const float epsilon = 0.0001f;
            return std::hash<float>()(std::round(vec.x / epsilon)) ^
                std::hash<float>()(std::round(vec.y / epsilon)) ^
                std::hash<float>()(std::round(vec.z / epsilon));
        }
    };

    struct Vec3Equal
    {
        bool operator()(const glm::vec3& a, const glm::vec3& b) const
        {
            const float epsilon = 0.0001f;
            return std::fabs(a.x - b.x) < epsilon &&
                std::fabs(a.y - b.y) < epsilon &&
                std::fabs(a.z - b.z) < epsilon;
        }
    };

    struct vectors
    {
        glm::vec3 normal = glm::vec3(0);
        glm::vec3 tangent = glm::vec3(0);
        glm::vec3 bitangent = glm::vec3(0);
    };

    std::unordered_map<glm::vec3, vectors, Vec3Hash, Vec3Equal> averages;
    for (const auto& pos : positions)
    {
        averages[pos] = { glm::vec3(0), glm::vec3(0), glm::vec3(0) };
    }

    std::vector<GLuint> indices;
    for (unsigned int h = 0; h < m_Count; h++)
    {
        for (unsigned int w = 0; w < m_Count; w++)
        {
            int c = w + h * (m_Count + 1);
            indices.push_back(c);
            indices.push_back(c + m_Count + 1);
            indices.push_back(c + m_Count + 2);
            indices.push_back(c);
            indices.push_back(c + m_Count + 2);
            indices.push_back(c + 1);

            auto pos = std::make_tuple(positions[c], positions[c + m_Count + 1], positions[c + m_Count + 2]);
            auto tex = std::make_tuple(textures[c], textures[c + m_Count + 1], textures[c + m_Count + 2]);
            auto tbn = Shape::GenerateTBN(pos, tex);

            averages[positions[c]].tangent += std::get<0>(tbn);
            averages[positions[c]].bitangent += std::get<1>(tbn);
            averages[positions[c]].normal += std::get<2>(tbn);

            averages[positions[c + m_Count + 1]].tangent += std::get<0>(tbn);
            averages[positions[c + m_Count + 1]].bitangent += std::get<1>(tbn);
            averages[positions[c + m_Count + 1]].normal += std::get<2>(tbn);

            averages[positions[c + m_Count + 2]].tangent += std::get<0>(tbn);
            averages[positions[c + m_Count + 2]].bitangent += std::get<1>(tbn);
            averages[positions[c + m_Count + 2]].normal += std::get<2>(tbn);

            pos = std::make_tuple(positions[c], positions[c + m_Count + 2], positions[c + 1]);
            tex = std::make_tuple(textures[c], textures[c + m_Count + 2], textures[c + 1]);
            tbn = Shape::GenerateTBN(pos, tex);

            averages[positions[c]].tangent += std::get<0>(tbn);
            averages[positions[c]].bitangent += std::get<1>(tbn);
            averages[positions[c]].normal += std::get<2>(tbn);

            averages[positions[c + m_Count + 2]].tangent += std::get<0>(tbn);
            averages[positions[c + m_Count + 2]].bitangent += std::get<1>(tbn);
            averages[positions[c + m_Count + 2]].normal += std::get<2>(tbn);

            averages[positions[c + 1]].tangent += std::get<0>(tbn);
            averages[positions[c + 1]].bitangent += std::get<1>(tbn);
            averages[positions[c + 1]].normal += std::get<2>(tbn);
        }
    }

    std::vector<Vertex> vertices(positions.size());
    for (unsigned int c = 0; c < positions.size(); c++)
    {
        auto vectors = averages.at(positions[c]);
        vertices[c] = Vertex(positions[c], glm::normalize(vectors.normal), glm::normalize(vectors.tangent), glm::normalize(vectors.bitangent), textures[c]);
    }

    std::cout << averages.size() << " " << positions.size() << std::endl;

    UploadToGpu(vertices, indices);
}

*/