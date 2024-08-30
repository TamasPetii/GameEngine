#include "Animation.h"

#define DEBUG
#ifdef DEBUG
#define LOG_ANIMATION(message) std::cout << "[Animation] : " << message << std::endl;
#define LOG_ANIMATION_FUN(function, message) std::cout << "[Animation] - {" << function << "} : " << message << std::endl;
#else
#define LOG_ANIMATION(function, message)
#define LOG_ANIMATION_FUN(function, message)
#endif

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

void Animation::Load(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        exit(1);
    }

    m_Path = path;

    auto animation = scene->mAnimations[0];
    m_BoneCount = 0;
    m_Duration = animation->mDuration;
    m_TicksPerSecond = animation->mTicksPerSecond;

    PreProcess(scene->mRootNode, scene);
    Process(scene->mRootNode, scene);
    ProcessHierarchy(scene->mRootNode);
    ProcessMissingBones(animation);
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
    std::cout << "[Animation::PreProcess] : VertexCount = " << m_VertexCount << std::endl;
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

        std::cout << "[Animation::Process] : CurrentNode = " << currentNode->mName.C_Str() << std::endl;

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
    std::cout << "[Animation::ProcessBone] : Process Started **********************" << std::endl;
    std::cout << "[Animation::ProcessBone] : BoneCount = " << m_BoneCount << std::endl;
    std::cout << "[Animation::ProcessBone] : VertexStart = " << count << std::endl;
    std::cout << "[Animation::ProcessBone] : BoneInfo Size = " << m_BoneInfos.size() << std::endl;

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

            std::cout << "[Animation::ProcessBone] : New Bone Found = " << name << std::endl;
            std::cout << "[Animation::ProcessBone] : New Bone Index = " << m_BoneInfos[name].index << std::endl;
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

                    //std::cout << "\t[Animation::ProcessBone] : New Vertex Weight = " << boneIndex << " " << weights[w].mWeight << std::endl;
                    break;
                }
            }
        }
    }

    count += mesh->mNumVertices;

	std::cout << "[Animation::ProcessBone] : BoneCount = " << m_BoneCount << std::endl;
	std::cout << "[Animation::ProcessBone] : VertexStart = " << count << std::endl;
	std::cout << "[Animation::ProcessBone] : BoneInfo Size = " << m_BoneInfos.size() << std::endl;
	std::cout << "[Animation::ProcessBone] : Process Ended **********************" << std::endl;
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
    std::cout << "[Animation::ProcessMissingBones] : Process Started *************" << std::endl;

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

        std::cout << "[Animation::ProcessMissingBones] : Bone added to m_Bones -------------------" << std::endl;
        std::cout << "[Animation::ProcessMissingBones] : Bone index in vector = " << m_Bones.size() - 1 << std::endl;
        std::cout << "[Animation::ProcessMissingBones] : Bone index = " << m_BoneInfos[name].index << std::endl;
        std::cout << "[Animation::ProcessMissingBones] : Bone name = " << name << std::endl;
    }

    std::cout << "[Animation::ProcessMissingBones] : Process Ended *************" << std::endl;
}