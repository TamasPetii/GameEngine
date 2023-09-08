#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include "Mesh.h"
#include <tuple>
#include <string>
#include <type_traits>
#include <unordered_set>

class Shape : public Mesh
{
public:
	Shape() = default;
	Shape(const Shape& other) = default;

	template<typename T>
	static T* Instance();

	inline const auto& Get_Name() const { return m_Name; }
protected:
	static std::tuple<glm::vec3, glm::vec3, glm::vec3> GenerateTBN(const std::tuple<glm::vec3, glm::vec3, glm::vec3>& positions, const std::tuple<glm::vec2, glm::vec2, glm::vec2>& textures);
	static std::pair<glm::vec3, glm::vec3> GenerateTB(const glm::vec3& normal);
protected:
	std::string m_Name;
	static std::unordered_set<Shape*> m_ShapeInstances;
};
/*
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
*/

template<typename T>
T* Shape::Instance()
{
	for (auto shape : m_ShapeInstances)
	{
		if (dynamic_cast<T*>(shape) != nullptr)
		{
			return dynamic_cast<T*>(shape);
		}
	}

	T* newShape = new T();
	m_ShapeInstances.insert(newShape);
	return newShape;
}