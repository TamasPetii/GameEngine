#pragma once
#include "EngineApi.h"
#include <queue>
#include <stack>
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "Registry/Component/MaterialComponent.h"
#include "Render/OpenGL/Vertex.h"
#include "Render/OpenGL/BufferGL.h"
#include "Render/OpenGL/VertexArrayGL.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"
#include "Model/AssimpConverter.h"

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;
};

class Bone
{
public:
    Bone(const std::string& name, int index, aiNodeAnim* channel);
    void Update(float animationTime);
    const auto& GetName() const { return m_Name; }
    const auto& GetIndex() const { return m_Index; }
    const auto& GetTransform() const { return m_Transform; }
private:
    void ProcessPositionKeys(aiNodeAnim* channel);
    void ProcessRotationKeys(aiNodeAnim* channel);
    void ProcessScaleKeys(aiNodeAnim* channel);
    int GetPositionIndex(float animationTime);
    int GetRotationIndex(float animationTime);
    int GetScaleIndex(float animationTime);
    glm::mat4 InterpolatePosition(float animationTime);
    glm::mat4 InterpolateRotation(float animationTime);
    glm::mat4 InterpolateScale(float animationTime);
    float GetFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
private:
    int m_Index;
    std::string m_Name;
    glm::mat4 m_Transform;
    std::vector<KeyScale> m_Scales;
    std::vector<KeyPosition> m_Positions;
    std::vector<KeyRotation> m_Rotations;
};

