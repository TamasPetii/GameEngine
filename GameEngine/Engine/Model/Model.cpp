#include "Model.h"

Model::Model()
{
    m_Vao = std::make_unique<VertexArrayGL>();
    m_Ibo = std::make_unique<IndexBufferGL>();
    m_Vbo = std::make_unique<VertexBufferGL>();
    m_MaterialSsbo = std::make_unique<ShaderStorageBufferGL>();
    m_InstanceSsbo = std::make_unique<ShaderStorageBufferGL>();
    m_ShadowInstanceSsbo = std::make_unique<ShaderStorageBufferGL>();
}

Model::~Model()
{

}

void Model::Load(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        exit(1);
    }

    LOG_DEBUG("Model", "Loading model started: " + path);
    
    hasAnimation = scene->HasAnimations();
    m_Path = path;
    m_Directory = path.substr(0, path.find_last_of('/'));

    PreProcess(scene->mRootNode, scene);
    Process(scene->mRootNode, scene);
    GenerateBuffers();
    GenerateObb();

    LOG_DEBUG("Model", std::filesystem::path(path).filename().string() + " model info: " + "Vertex Count = " + std::to_string(m_VertexCount) + " | " + "Index Count = " + std::to_string(m_IndexCount));
    LOG_DEBUG("Model", "Loading model was successful: " + path);
}

void Model::PreProcess(aiNode* node, const aiScene* scene)
{
    std::queue<aiNode*> queue;
    queue.push(node);

    while (!queue.empty())
    {
        aiNode* currentNode = queue.front();
        queue.pop();

        for (unsigned int i = 0; i < currentNode->mNumMeshes; ++i)
        {
            aiMesh* currentMesh = scene->mMeshes[currentNode->mMeshes[i]];
            m_IndexCount += currentMesh->mNumFaces * 3;
            m_VertexCount += currentMesh->mNumVertices;
            m_MeshCount++;
        }

        for (unsigned int i = 0; i < currentNode->mNumChildren; ++i)
        {
            queue.push(currentNode->mChildren[i]);
        }
    }

    m_VertexPositions.reserve(m_VertexCount);
    m_VertexIndices.reserve(m_IndexCount);

    m_Vertices.reserve(m_VertexCount);
    m_Indices.reserve(m_IndexCount);
    m_Materials.reserve(m_MeshCount);
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
    unsigned int start_mesh_index = count++;
    unsigned int start_face_index = m_Vertices.size();

    //Geometry Data Process
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

        m_Vertices.emplace_back(Vertex(position, normal, tangent, texcoord, start_mesh_index));
        m_VertexPositions.push_back(position);
    }

    //Index Data
    for (int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j)
        {
            m_Indices.emplace_back(start_face_index + face.mIndices[j]);
            m_VertexIndices.push_back(start_face_index + face.mIndices[j]);
        }
    }

    TextureManager* textureManager = TextureManager::Instance();
    MaterialComponent materialComponent;
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

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

            if(materialComponent.diffuse)
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

            if(materialComponent.normal)
                LOG_DEBUG("Model", "Normal texture loaded for model " + std::filesystem::path(m_Path).filename().string() + ": " + materialComponent.diffuse->GetPath());
        }

        //Normals texture for obj
        if (material->GetTextureCount(aiTextureType_HEIGHT) > 0)
        {
            aiString path;
            material->GetTexture(aiTextureType_HEIGHT, 0, &path);
            std::string real_path = m_Directory + "/" + std::string(path.C_Str());

            if (textureManager->IsImageTextureLoaded(real_path))
                materialComponent.normal = textureManager->GetImageTexture(real_path);
            else
                materialComponent.normal = textureManager->LoadImageTexture(real_path);

            if(materialComponent.normal)
                LOG_DEBUG("Model", "Height texture loaded for model " + std::filesystem::path(m_Path).filename().string() + ": " + materialComponent.diffuse->GetPath());
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

            if(materialComponent.specular)
                LOG_DEBUG("Model", "Specular texture loaded for model " + std::filesystem::path(m_Path).filename().string() + ": " + materialComponent.diffuse->GetPath());
        }

        aiColor3D diffuseColor(1.f, 1.f, 1.f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
        materialComponent.color = glm::vec4(Assimp::ConvertAssimpToGlm(diffuseColor), 1);

        m_Materials.push_back(materialComponent); 
    }
}

void Model::GenerateBuffers()
{
    std::vector<MaterialGLSL> materials;
    materials.reserve(m_Materials.size());

    for (const auto& material : m_Materials)
        materials.emplace_back(MaterialGLSL(material));

    m_MaterialSsbo->BufferStorage(materials.size() * sizeof(MaterialGLSL), materials.data(), GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

    m_Vbo->BufferStorage(m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_NONE);
    m_Ibo->BufferStorage(m_Indices.size() * sizeof(GLuint), m_Indices.data(), GL_NONE);
    m_Vao->AttachVertexBuffer(m_Vbo, sizeof(Vertex), 0);
    m_Vao->AttachIndexBuffer(m_Ibo);
    m_Vao->LinkAttribute(0, 0, 3, GL_FLOAT, (GLuint)(0 * sizeof(glm::vec3)));
    m_Vao->LinkAttribute(0, 1, 3, GL_FLOAT, (GLuint)(1 * sizeof(glm::vec3)));
    m_Vao->LinkAttribute(0, 2, 3, GL_FLOAT, (GLuint)(2 * sizeof(glm::vec3)));
    m_Vao->LinkAttribute(0, 3, 2, GL_FLOAT, (GLuint)(3 * sizeof(glm::vec3)));
    m_Vao->LinkAttributeI(0, 4, 1, GL_UNSIGNED_INT, (GLuint)(3 * sizeof(glm::vec3) + 1 * sizeof(glm::vec2)));

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
        m_ShadowInstanceSsbo->BufferData(m_ShadowInstances.size() * sizeof(GLuint), m_ShadowInstances.data(), GL_DYNAMIC_DRAW);
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