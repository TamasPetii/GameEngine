#include "Bone.h"

Bone::Bone(const std::string& name, int index, aiNodeAnim* channel) :
	m_Name(name),
	m_Index(index)
{
	m_Positions.reserve(channel->mNumPositionKeys);
	m_Rotations.reserve(channel->mNumRotationKeys);
	m_Scales.reserve(channel->mNumScalingKeys);

    ProcessPositionKeys(channel);
    ProcessRotationKeys(channel);
    ProcessScaleKeys(channel);
}

void Bone::ProcessPositionKeys(aiNodeAnim* channel)
{
    for (int positionIndex = 0; positionIndex < channel->mNumPositionKeys; ++positionIndex)
    {
        KeyPosition data;
        data.position = Assimp::ConvertAssimpToGlm(channel->mPositionKeys[positionIndex].mValue);
        data.timeStamp = channel->mPositionKeys[positionIndex].mTime;
        m_Positions.emplace_back(std::move(data));
    }
}

void Bone::ProcessRotationKeys(aiNodeAnim* channel)
{
    for (int rotationIndex = 0; rotationIndex < channel->mNumRotationKeys; ++rotationIndex)
    {
        KeyRotation data;
        data.orientation = Assimp::ConvertAssimpToGlm(channel->mRotationKeys[rotationIndex].mValue);
        data.timeStamp = channel->mRotationKeys[rotationIndex].mTime;
        m_Rotations.emplace_back(std::move(data));
    }
}

void Bone::ProcessScaleKeys(aiNodeAnim* channel)
{
    for (int scaleIndex = 0; scaleIndex < channel->mNumScalingKeys; ++scaleIndex)
    {
        KeyScale data;
        data.scale = Assimp::ConvertAssimpToGlm(channel->mScalingKeys[scaleIndex].mValue);
        data.timeStamp = channel->mScalingKeys[scaleIndex].mTime;
        m_Scales.emplace_back(std::move(data));
    }
}

int Bone::GetPositionIndex(float animationTime)
{
    for (int i = 0; i < m_Positions.size() - 1; ++i)
    {
        if (animationTime < m_Positions[i + 1].timeStamp)
            return i;
    }
    return -1;
}

int Bone::GetRotationIndex(float animationTime)
{
    for (int i = 0; i < m_Rotations.size() - 1; ++i)
    {
        if (animationTime < m_Rotations[i + 1].timeStamp)
            return i;
    }
    return -1;
}

int Bone::GetScaleIndex(float animationTime)
{
    for (int i = 0; i < m_Scales.size() - 1; ++i)
    {
        if (animationTime < m_Scales[i + 1].timeStamp)
            return i;
    }
    return -1;
}


glm::mat4 Bone::InterpolatePosition(float animationTime)
{
    if (m_Positions.size() == 1)
        return glm::translate(m_Positions[0].position);

    int p0Index = GetPositionIndex(animationTime);
    int p1Index = p0Index + 1;
    float factor = GetFactor(m_Positions[p0Index].timeStamp, m_Positions[p1Index].timeStamp, animationTime);
    glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].position, m_Positions[p1Index].position, factor);
    return glm::translate(finalPosition);
}

glm::mat4 Bone::InterpolateRotation(float animationTime)
{
    if (m_Rotations.size() == 1)
        return glm::toMat4(glm::normalize(m_Rotations[0].orientation));

    int p0Index = GetRotationIndex(animationTime);
    int p1Index = p0Index + 1;
    float factor = GetFactor(m_Rotations[p0Index].timeStamp, m_Rotations[p1Index].timeStamp, animationTime);
    glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].orientation, m_Rotations[p1Index].orientation, factor);
    return glm::toMat4(glm::normalize(finalRotation));
}

glm::mat4 Bone::InterpolateScale(float animationTime)
{
    if (m_Scales.size() == 1)
        return glm::scale(m_Scales[0].scale);

    int p0Index = GetScaleIndex(animationTime);
    int p1Index = p0Index + 1;
    float factor = GetFactor(m_Scales[p0Index].timeStamp, m_Scales[p1Index].timeStamp, animationTime);
    glm::vec3 finalScale = glm::mix(m_Scales[p0Index].scale, m_Scales[p1Index].scale, factor);
    return glm::scale(finalScale);
}

float Bone::GetFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    return (animationTime - lastTimeStamp) / (nextTimeStamp - lastTimeStamp);
}

void Bone::Update(float animationTime)
{
    glm::mat4 translation = InterpolatePosition(animationTime);
    glm::mat4 rotation = InterpolateRotation(animationTime);
    glm::mat4 scale = InterpolateScale(animationTime);
    m_Transform = translation * rotation * scale;
}