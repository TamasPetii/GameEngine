#include "Model.h"
#include "AssimpConverter.h"
#include <Assimp/Importer.hpp>
#include <Assimp/postprocess.h>
#include <queue>
#include <filesystem>
#include <meshoptimizer.h>

#include "Logger/Logger.h"
#include "Manager/TextureManager.h"
#include "Registry/Component/Object/MaterialComponent.h"

#include "Render/OpenGL/Vertex.h"
#include "Render/OpenGL/BufferGL.h"
#include "Render/OpenGL/TextureGL.h"
#include "Render/OpenGL/VertexArrayGL.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"

Model::Model() :
    m_Vao{ std::make_unique<VertexArrayGL>() },
    m_Ibo{ std::make_unique<IndexBufferGL>() },
    m_Vbo{ std::make_unique<VertexBufferGL>() },
    m_MaterialSsbo{ std::make_unique<ShaderStorageBufferGL>() },
    m_InstanceSsbo{ std::make_unique<ShaderStorageBufferGL>() },
    m_ShadowInstanceSsbo{ std::make_unique<ShaderStorageBufferGL>() },
    m_MeshCount{ 0 },
    m_IndexCount{ 0 },
    m_VertexCount{ 0 },
    m_LodLevels{ 4 },
    hasAnimation{ false }
{
}

Model::~Model()
{
}

bool Model::Load(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG_ERROR("ModelManager", "Couldn't load model! Path = " + path);
        return false;
    }

    LOG_DEBUG("Model", "Loading model started: " + path);
    
    hasAnimation = scene->HasAnimations();
    m_Path = path;
    m_Directory = path.substr(0, path.find_last_of('/'));

    Process(scene->mRootNode, scene);
    GenerateBuffers();
    GenerateObb();

    LOG_DEBUG("Model", std::filesystem::path(path).filename().string() + " model info: " + "Vertex Count = " + std::to_string(m_VertexCount) + " | " + "Index Count = " + std::to_string(m_IndexCount));
    LOG_DEBUG("Model", "Loading model was successful: " + path);

    return true;
}

void Model::Process(aiNode* node, const aiScene* scene)
{
    std::queue<aiNode*> queue;
    queue.push(node);
    unsigned int count = 0;

    while (!queue.empty())
    {
        aiNode* currentNode = queue.front();
        queue.pop();

        for (unsigned int i = 0; i < currentNode->mNumMeshes; ++i)
        {
            aiMesh* currentMesh = scene->mMeshes[currentNode->mMeshes[i]];
            ProcessGeometry(currentMesh, scene, count);
        }

        for (unsigned int i = 0; i < currentNode->mNumChildren; ++i)
        {
            queue.push(currentNode->mChildren[i]);
        }
    }
}

void Model::ProcessGeometry(aiMesh* mesh, const aiScene* scene, unsigned int& count)
{
    TextureManager* textureManager = TextureManager::Instance();
    unsigned int start_mesh_index = count++;

    std::string materialName = "";
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        materialName = std::string(material->GetName().C_Str());

        if (m_FoundMaterials.find(materialName) == m_FoundMaterials.end())
        {
            MaterialComponent materialComponent;

            //Diffuse texture
            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiString path;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
                std::string real_path = m_Directory + "/" + std::string(path.C_Str());

                if (textureManager->IsImageTextureLoaded(real_path))
                    materialComponent.diffuse = textureManager->GetImageTexture(real_path);
                else
                    materialComponent.diffuse = textureManager->LoadImageTexture(real_path);

                if (materialComponent.diffuse)
                    LOG_DEBUG("Model", "Diffuse texture loaded for model " + std::filesystem::path(m_Path).filename().string() + ": " + materialComponent.diffuse->GetPath());
            }

            //Normals texture
            if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
            {
                aiString path;
                material->GetTexture(aiTextureType_NORMALS, 0, &path);
                std::string real_path = m_Directory + "/" + std::string(path.C_Str());

                if (textureManager->IsImageTextureLoaded(real_path))
                    materialComponent.normal = textureManager->GetImageTexture(real_path);
                else
                    materialComponent.normal = textureManager->LoadImageTexture(real_path);

                if (materialComponent.normal)
                    LOG_DEBUG("Model", "Normal texture loaded for model " + std::filesystem::path(m_Path).filename().string() + ": " + materialComponent.diffuse->GetPath());
            }

            //Height texture
            if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
            {
                aiString path;
                material->GetTexture(aiTextureType_HEIGHT, 0, &path);
                std::string real_path = m_Directory + "/" + std::string(path.C_Str());

                if (textureManager->IsImageTextureLoaded(real_path))
                    materialComponent.normal = textureManager->GetImageTexture(real_path);
                else
                    materialComponent.normal = textureManager->LoadImageTexture(real_path);

                if (materialComponent.normal)
                    LOG_DEBUG("Model", "Height texture loaded for model " + std::filesystem::path(m_Path).filename().string() + ": " + materialComponent.diffuse->GetPath());
            }

            //Displacement
            if (material->GetTextureCount(aiTextureType_DISPLACEMENT) > 0)
            {
                aiString path;
                material->GetTexture(aiTextureType_DISPLACEMENT, 0, &path);
                std::string real_path = m_Directory + "/" + std::string(path.C_Str());

                if (textureManager->IsImageTextureLoaded(real_path))
                    materialComponent.normal = textureManager->GetImageTexture(real_path);
                else
                    materialComponent.normal = textureManager->LoadImageTexture(real_path);

                if (materialComponent.normal)
                    LOG_DEBUG("Model", "Displacement texture loaded for model " + std::filesystem::path(m_Path).filename().string() + ": " + materialComponent.diffuse->GetPath());
            }

            //Specular texture
            if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
            {
                aiString path;
                material->GetTexture(aiTextureType_SPECULAR, 0, &path);
                std::string real_path = m_Directory + "/" + std::string(path.C_Str());

                if (textureManager->IsImageTextureLoaded(real_path))
                    materialComponent.specular = textureManager->GetImageTexture(real_path);
                else
                    materialComponent.specular = textureManager->LoadImageTexture(real_path);

                if (materialComponent.specular)
                    LOG_DEBUG("Model", "Specular texture loaded for model " + std::filesystem::path(m_Path).filename().string() + ": " + materialComponent.diffuse->GetPath());
            }

            aiColor3D diffuseColor(1.f, 1.f, 1.f);
            material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
            materialComponent.color = glm::vec4(Assimp::ConvertAssimpToGlm(diffuseColor), 1);

            m_FoundMaterials[materialName] = m_Materials.size();
            m_Materials.push_back(materialComponent);
        }
    }

    //Vertex Data Process
    std::vector<Vertex> vertices;
    for (int i = 0; i < mesh->mNumVertices; ++i)
    {
        //Position
        glm::vec3 position{ 0,0,0 };
        if (mesh->mVertices)
            position = Assimp::ConvertAssimpToGlm(mesh->mVertices[i]);

        //Normals
        glm::vec3 normal{ 0,0,0 };
        if (mesh->mNormals)
            normal = Assimp::ConvertAssimpToGlm(mesh->mNormals[i]);

        //Tangents
        glm::vec3 tangent{ 0,0,0 };
        if (mesh->mTangents)
            tangent = Assimp::ConvertAssimpToGlm(mesh->mTangents[i]);

        //Textures
        glm::vec2 texcoord{ 0,0 };
        if (mesh->mTextureCoords[0] != nullptr)
        {
            texcoord.x = mesh->mTextureCoords[0][i].x;
            texcoord.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.emplace_back(position, normal, tangent, texcoord, m_FoundMaterials[materialName]);
    }

    //Index Data
    std::vector<unsigned int> indices;
    for (int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    m_LodIndices.resize(m_LodLevels);
    m_LodThresholds = { 1.f, 0.5f, 0.25f, 0.10f };

    for (int i = 0; i < m_LodLevels; i++)
    {
        float threshold = m_LodThresholds[i];
        size_t target_index_count = size_t(indices.size() * threshold);
        float target_error = 0.02f;
        float lod_error = 0.f;

        std::vector<unsigned int> simplifiedIndices(indices.size());
        size_t simplifiedIndexCount = meshopt_simplify(simplifiedIndices.data(), indices.data(), indices.size(), &(vertices[0].position.x), vertices.size(), sizeof(Vertex), target_index_count, target_error, 0, &lod_error);
        simplifiedIndices.resize(simplifiedIndexCount);

        unsigned int start_face_index = m_Vertices.size();
        for (int i = 0; i < simplifiedIndices.size(); i++)
        {
            simplifiedIndices[i] += start_face_index;
        }

        m_LodIndices[i].insert(m_LodIndices[i].end(), simplifiedIndices.begin(), simplifiedIndices.end());
    }

    m_Vertices.insert(m_Vertices.end(), vertices.begin(), vertices.end());
    for (int i = 0; i < vertices.size(); i++)
        m_VertexPositions.push_back(vertices[i].position);

    /*
    size_t index_count = indices.size();
    size_t unindexed_vertex_count = vertices.size();
    std::vector<unsigned int> remap(index_count);
    size_t vertex_count = meshopt_generateVertexRemap(&remap[0], indices.data(), index_count, vertices.data(), unindexed_vertex_count, sizeof(Vertex));

    std::vector<unsigned int> OptIndices;
    std::vector<Vertex> OptVertices;
    OptIndices.resize(index_count);
    OptVertices.resize(vertex_count);

    meshopt_remapIndexBuffer(OptIndices.data(), indices.data(), index_count, remap.data());
    meshopt_remapVertexBuffer(OptVertices.data(), vertices.data(), unindexed_vertex_count, sizeof(Vertex), remap.data());
    meshopt_optimizeVertexCache(OptIndices.data(), OptIndices.data(), index_count, vertex_count);
    meshopt_optimizeOverdraw(OptIndices.data(), OptIndices.data(), index_count, &(OptVertices[0].position.x), vertex_count, sizeof(Vertex), 1.05f);
    meshopt_optimizeVertexFetch(OptVertices.data(), OptIndices.data(), index_count, OptVertices.data(), vertex_count, sizeof(Vertex));

    m_LodIndices.resize(m_LodLevels);
    m_LodThresholds = { 1.f, 0.5f, 0.25f, 0.125f };

    for (int i = 0; i < m_LodLevels; i++)
    {
        float threshold = m_LodThresholds[i];
        size_t target_index_count = size_t(index_count * threshold);
        float target_error = 0.02f;
        float lod_error = 0.f;

        std::vector<unsigned int> simplifiedIndices(index_count);
        size_t simplifiedIndexCount = meshopt_simplify(simplifiedIndices.data(), OptIndices.data(), index_count, &(OptVertices[0].position.x), vertex_count, sizeof(Vertex), target_index_count, target_error, 0, &lod_error);
        simplifiedIndices.resize(simplifiedIndexCount);

        unsigned int start_face_index = m_Vertices.size();
        for (int i = 0; i < simplifiedIndices.size(); i++)
        {
            simplifiedIndices[i] += start_face_index;
        }

        m_LodIndices[i].insert(m_LodIndices[i].end(), simplifiedIndices.begin(), simplifiedIndices.end());
    }

    m_Vertices.insert(m_Vertices.end(), OptVertices.begin(), OptVertices.end());
    for (int i = 0; i < OptVertices.size(); i++)
        m_VertexPositions.push_back(OptVertices[i].position);
    */
}

void Model::GenerateBuffers()
{
    m_VertexIndices = m_LodIndices[0];
    m_LodIndicesSize.resize(m_LodLevels);
    m_LodIndicesOffsets.resize(m_LodLevels);
    for (int i = 0; i < m_LodLevels; i++)
    {
        m_LodIndicesOffsets[i] = m_Indices.size();
        m_LodIndicesSize[i] = m_LodIndices[i].size();
        m_Indices.insert(m_Indices.end(), m_LodIndices[i].begin(), m_LodIndices[i].end());
        m_LodIndices[i].clear();
        m_LodIndices.shrink_to_fit();
    }

    std::vector<MaterialGLSL> materials;
    materials.reserve(m_Materials.size());

    for (const auto& material : m_Materials)
        materials.emplace_back(MaterialGLSL(material));

    m_MaterialSsbo->BufferStorage(materials.size() * sizeof(MaterialGLSL), materials.data(), GL_NONE);

    m_Vbo->BufferStorage(m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_NONE);
    m_Ibo->BufferStorage(m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_NONE);
    m_Vao->AttachVertexBuffer(m_Vbo, sizeof(Vertex), 0);
    m_Vao->AttachIndexBuffer(m_Ibo);
    m_Vao->LinkAttribute(0, 0, 3, GL_FLOAT, (unsigned int)(0 * sizeof(glm::vec3)));
    m_Vao->LinkAttribute(0, 1, 3, GL_FLOAT, (unsigned int)(1 * sizeof(glm::vec3)));
    m_Vao->LinkAttribute(0, 2, 3, GL_FLOAT, (unsigned int)(2 * sizeof(glm::vec3)));
    m_Vao->LinkAttribute(0, 3, 2, GL_FLOAT, (unsigned int)(3 * sizeof(glm::vec3)));
    m_Vao->LinkAttributeI(0, 4, 1, GL_UNSIGNED_INT, (unsigned int)(3 * sizeof(glm::vec3) + 1 * sizeof(glm::vec2)));

    m_VertexCount = m_Vertices.size();
    m_IndexCount = m_Indices.size();

    m_Vertices.clear();
    m_Vertices.shrink_to_fit();
    m_Indices.clear();
    m_Indices.shrink_to_fit();
}

void Model::Bind()
{
    m_Vao->Bind();
}

void Model::UnBind()
{
    m_Vao->UnBind();
}

void Model::UpdateInstanceSsbo()
{
    if (m_Instances.size() != 0)
        m_InstanceSsbo->BufferData(m_Instances.size() * sizeof(glm::uvec4), m_Instances.data(), GL_DYNAMIC_DRAW);
}

void Model::UpdateShadowInstanceSsbo()
{
    if (m_ShadowInstances.size() != 0)
        m_ShadowInstanceSsbo->BufferData(m_ShadowInstances.size() * sizeof(unsigned int), m_ShadowInstances.data(), GL_DYNAMIC_DRAW);
}

void Model::GenerateObb()
{
    glm::vec3 maxPosition{ std::numeric_limits<float>::lowest() };
    glm::vec3 minPosition{ std::numeric_limits<float>::max() };

    for (auto& position : m_VertexPositions)
    {
        if (position.x > maxPosition.x) maxPosition.x = position.x;
        if (position.y > maxPosition.y) maxPosition.y = position.y;
        if (position.z > maxPosition.z) maxPosition.z = position.z;
        if (position.x < minPosition.x) minPosition.x = position.x;
        if (position.y < minPosition.y) minPosition.y = position.y;
        if (position.z < minPosition.z) minPosition.z = position.z;
    }

    m_Obb[0] = glm::vec3(maxPosition.x, maxPosition.y, maxPosition.z);
    m_Obb[1] = glm::vec3(maxPosition.x, maxPosition.y, minPosition.z);
    m_Obb[2] = glm::vec3(maxPosition.x, minPosition.y, maxPosition.z);
    m_Obb[3] = glm::vec3(maxPosition.x, minPosition.y, minPosition.z);
    m_Obb[4] = glm::vec3(minPosition.x, maxPosition.y, maxPosition.z);
    m_Obb[5] = glm::vec3(minPosition.x, maxPosition.y, minPosition.z);
    m_Obb[6] = glm::vec3(minPosition.x, minPosition.y, maxPosition.z);
    m_Obb[7] = glm::vec3(minPosition.x, minPosition.y, minPosition.z);

    m_ObbMax = maxPosition;
    m_ObbMin = minPosition;
    m_ObbOrigin = 0.5f * (minPosition + maxPosition);
    m_ObbExtents = 0.5f * (maxPosition - minPosition);
}