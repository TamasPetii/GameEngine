#include "Animation.h"

Animation::Animation() : 
	m_MeshCount(0),
    m_BoneCount(0),
    m_VertexCount(0),
    m_Duration(0),
    m_TicksPerSecond(0),
    root(nullptr),
    m_VertexBoneInfoSsbo(nullptr)
{

}

bool Animation::Load(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG_ERROR("ModelManager", "Couldn't load animation! Path = " + path);
        return false;
    }

    if (!scene->HasAnimations())
    {
        LOG_ERROR("ModelManager", "Loaded model has no animation! Path = " + path);
        return false;
    }

    LOG_DEBUG("Animation", "Loading animation started: " + path);

    m_Path = path;
    auto animation = scene->mAnimations[0];
    m_BoneCount = 0;
    m_Duration = animation->mDuration;
    m_TicksPerSecond = animation->mTicksPerSecond;

    PreProcess(scene->mRootNode, scene);
    Process(scene->mRootNode, scene);
    ProcessHierarchy(scene->mRootNode);
    ProcessMissingBones(animation);

    LOG_DEBUG("Animation", std::filesystem::path(path).filename().string() + " animation info: Bone Count = " + std::to_string(m_BoneCount) + " | Vertex Count = " + std::to_string(m_VertexCount));
    LOG_DEBUG("Animation", "Loading animation was successful: " + path);
    
    return true;
}

void Animation::PreProcess(aiNode* node, const aiScene* scene)
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
            m_VertexCount += currentMesh->mNumVertices;
            m_MeshCount++;
        }

        for (unsigned int i = 0; i < currentNode->mNumChildren; ++i)
        {
            queue.push(currentNode->mChildren[i]);
        }
    }

    m_VertexBoneData.resize(m_VertexCount);
}

void Animation::Process(aiNode* node, const aiScene* scene)
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
            ProcessBone(currentMesh, scene, count);
        }

        for (unsigned int i = 0; i < currentNode->mNumChildren; ++i)
        {
            queue.push(currentNode->mChildren[i]);
        }
    }

    m_VertexBoneInfoSsbo = std::make_shared<ShaderStorageBufferGL>();
    m_VertexBoneInfoSsbo->BufferStorage(m_VertexBoneData.size() * sizeof(VertexBoneData), m_VertexBoneData.data(), GL_NONE);
}

void Animation::ProcessBone(aiMesh* mesh, const aiScene* scene, unsigned int& count)
{
    for (int i = 0; i < mesh->mNumBones; ++i)
    {
        int boneIndex = -1;
        std::string name = std::string(mesh->mBones[i]->mName.C_Str());

        //Create bone info if it is not in std::unordered_map
        //It means bone was not processed previously
        if (m_BoneInfos.find(name) == m_BoneInfos.end())
        {
            BoneInfo boneInfo;
            boneInfo.index = m_BoneCount++;
            boneInfo.offset = Assimp::ConvertAssimpToGlm(mesh->mBones[i]->mOffsetMatrix);
            m_BoneInfos[name] = boneInfo;

            LOG_DEBUG("Animation", "New bone found: Name = " + name + " | " + "Index = " + std::to_string(m_BoneInfos[name].index));
        }

        boneIndex = m_BoneInfos[name].index;

        //This is a list of all the vertex weights the bone if affecting in mesh
        auto weights = mesh->mBones[i]->mWeights;
        for (int w = 0; w < mesh->mBones[i]->mNumWeights; ++w)
        {
            //mVertexId is local to the mesh, meaning batching would not work
            GLint vertexID = weights[w].mVertexId + count;

            for (int i = 0; i < 4; i++)
            {
                if (m_VertexBoneData[vertexID].indices[i] == -1)
                {
                    m_VertexBoneData[vertexID].indices[i] = boneIndex;
                    m_VertexBoneData[vertexID].weights[i] = weights[w].mWeight;
                    break;
                }
            }
        }
    }

    count += mesh->mNumVertices;
}

void Animation::ProcessHierarchy(aiNode* node)
{
    std::queue<std::pair<std::shared_ptr<NodeData>, aiNode*>> queue;
    root = std::make_shared<NodeData>();
    queue.push(std::make_pair(root, node));

    while (!queue.empty())
    {
        auto [nodeData, nodeAi] = queue.front();
        queue.pop();

        nodeData->name = nodeAi->mName.data;
        nodeData->transform = Assimp::ConvertAssimpToGlm(nodeAi->mTransformation);

        for (int i = 0; i < nodeAi->mNumChildren; ++i)
        {
            nodeData->children.push_back(std::make_shared<NodeData>());
            queue.push(std::make_pair(nodeData->children[i], nodeAi->mChildren[i]));
        }
    }
}

void Animation::ProcessMissingBones(aiAnimation* animation)
{
    for (int i = 0; i < animation->mNumChannels; i++)
    {
        auto channel = animation->mChannels[i];
        std::string name = channel->mNodeName.data;

        if (m_BoneInfos.find(name) == m_BoneInfos.end())
        {
            BoneInfo boneInfo;
            boneInfo.index = m_BoneCount++;
            m_BoneInfos[name] = std::move(boneInfo);
        }

        m_Bones.emplace_back(Bone(name, m_BoneInfos[name].index, channel));
    }
}