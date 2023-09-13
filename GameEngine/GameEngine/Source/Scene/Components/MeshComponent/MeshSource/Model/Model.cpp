#include "Model.h"

Model* Model::LoadModel(const std::string& path)
{
    if (LOADED_MODELS.find(path) == LOADED_MODELS.end())
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

        if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
        {
            std::runtime_error("[Model::LoadModel]");
        }

        Model* model = new Model();
        model->m_Path = path;
        model->ProcessNode(scene->mRootNode, scene);
        LOADED_MODELS[path] = model;
    }

    return LOADED_MODELS[path];
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Parts.push_back(ProcessMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

ModelPart* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    Material meshMaterial{};
    Textures meshTextures{};

    //Vertex Data
    for (int i = 0; i < mesh->mNumVertices; ++i)
    {
        //Position
        glm::vec3 position;
        if(mesh->mVertices)
            position = Assimp::ConvertAssimpToGlm(mesh->mVertices[i]);

        //Normals
        glm::vec3 normal;
        if (mesh->mNormals)
            normal = Assimp::ConvertAssimpToGlm(mesh->mNormals[i]);

        //Tangent
        glm::vec3 tangent;
        if (mesh->mTangents)
            tangent = Assimp::ConvertAssimpToGlm(mesh->mTangents[i]);

        //Tangent
        glm::vec3 bitangent;
        if (mesh->mBitangents)
            bitangent = Assimp::ConvertAssimpToGlm(mesh->mBitangents[i]);

        //Textures
        glm::vec2 texture = glm::vec2(0);
        if (mesh->mTextureCoords[0] != nullptr)
        {
            texture.x = mesh->mTextureCoords[0][i].x;
            texture.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(Vertex(position, normal, tangent, bitangent, texture));
    }

    //Index Data
    for (int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    //Material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        //TEXTURE MATERIALS
        for (int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); ++i)
        {
            aiString path;
            material->GetTexture(aiTextureType_DIFFUSE, i, &path);
            meshTextures.diffuse = ImageTexture::LoadTexture(path.C_Str());
        }
        for (int i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); ++i)
        {
            aiString path;
            material->GetTexture(aiTextureType_SPECULAR, i, &path);
            meshTextures.specular = ImageTexture::LoadTexture(path.C_Str());
        }
        for (int i = 0; i < material->GetTextureCount(aiTextureType_NORMALS); ++i)
        {
            aiString path;
            material->GetTexture(aiTextureType_NORMALS, i, &path);
            meshTextures.normal = ImageTexture::LoadTexture(path.C_Str());
        }
        //TODO height
        

        //COLOR MATERIALS
        aiColor3D ambientColor(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
        meshMaterial.ambient = Assimp::ConvertAssimpToGlm(ambientColor);

        aiColor3D diffuseColor(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
        meshMaterial.diffuse = Assimp::ConvertAssimpToGlm(diffuseColor);

        aiColor3D specularColor(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
        meshMaterial.specular = Assimp::ConvertAssimpToGlm(specularColor);
    }

    std::cout << "--------Mesh---------" << std::endl;
    std::cout << "Vertices: " << vertices.size() << std::endl;
    std::cout << "Indices: " << indices.size() << std::endl;

    return new ModelPart(vertices, indices, meshMaterial, meshTextures);
}

void Model::Render() const
{
    /*
    for (auto part : m_Parts)
    {
        part->Render();
    }
    */
}