#pragma once
#include <JSON/json.hpp>
#include <GLM/glm.hpp>

#define Vec3_ToJson(vec) { {"X", vec.x}, {"Y", vec.y}, {"Z", vec.z} }
#define Vec2_ToJson(vec) { {"X", vec.x}, {"Y", vec.y} }
#define Vec3_FromJson(vec) glm::vec3(vec["X"], vec["Y"], vec["Z"])
#define Vec2_FromJson(vec) glm::vec2(vec["X"], vec["Y"])

using json = nlohmann::json;

class ISerializable
{
public:
	virtual json Serialize() const = 0;
	virtual void DeSerialize(const json& data) = 0;
};

