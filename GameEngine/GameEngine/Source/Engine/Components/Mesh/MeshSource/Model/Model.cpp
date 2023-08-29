#include "Model.h"

std::unordered_map<std::string, Model*> Model::m_LoadedModels;

Model* Model::LoadModel(const std::string& path)
{
    if (m_LoadedModels.find(path) == m_LoadedModels.end())
    {
	    Assimp::Importer importer;
	    const aiScene* scene = importer.ReadFile(path, aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

        if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
        {
            std::runtime_error("[Model::LoadModel]");
        }

        Model* model = new Model();
        model->mPath = path;
        model->ProcessNode(scene->mRootNode, scene);
        m_LoadedModels[path] = model;
    }
    
    return m_LoadedModels[path];
}


void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        mParts.push_back(ProcessMesh(mesh, scene));
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
    ModelMaterial modelMaterial;

    //Vertex Data
    for (int i = 0; i < mesh->mNumVertices; ++i)
    {
        //Position
        glm::vec3 position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;

        //Normals
        glm::vec3 normal = glm::vec3(1);
        if (mesh->mNormals != nullptr)
        {
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
        }

        //Textures
        glm::vec2 texture = glm::vec2(0);
        if (mesh->mTextureCoords[0] != nullptr)
        {
            texture.x = mesh->mTextureCoords[0][i].x;
            texture.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(Vertex(position, normal, texture));
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

        /*
        //TEXTURE MATERIALS
        for (int i = 0; i < material->GetTextureCount(aiTextureType_AMBIENT); ++i)
        {
            aiString path;
            material->GetTexture(aiTextureType_AMBIENT, i, &path);
            materials.push_back(new MeshMaterial("ambinet", m_Directory + "/" + path.C_Str()));
        }
        for (int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); ++i)
        {
            aiString path;
            material->GetTexture(aiTextureType_DIFFUSE, i, &path);
            materials.push_back(new MeshMaterial("diffuse", m_Directory + "/" + path.C_Str()));
        }
        for (int i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); ++i)
        {
            aiString path;
            material->GetTexture(aiTextureType_SPECULAR, i, &path);
            materials.push_back(new MeshMaterial("specular", m_Directory + "/" + path.C_Str()));
        }
        */

        //COLOR MATERIALS
        aiColor3D ambientColor(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
        modelMaterial.ambient = glm::vec3(ambientColor.r, ambientColor.g, ambientColor.b);

        aiColor3D diffuseColor(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
        modelMaterial.diffuse = glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);

        aiColor3D specularColor(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
        modelMaterial.specular = glm::vec3(specularColor.r, specularColor.g, specularColor.b);
    }

    std::cout << "--------Mesh---------" << std::endl;
    std::cout << "Vertices: " << vertices.size() << std::endl;
    std::cout << "Indices: " << indices.size() << std::endl;

    static int num = 0;

    return new ModelPart(vertices, indices, modelMaterial, num++);
}

void Model::Render(IFrameBufferObject* frameBuffer, Program* shaderProgram, const glm::mat4& transform)
{
    for (auto part : mParts)
    {
        part->RenderPart(frameBuffer, shaderProgram, transform);
    }
}