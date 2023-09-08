#pragma once
#include "../Shape.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <unordered_set>


class Sphere : public Shape
{
public:
	Sphere();
	Sphere(const Sphere& other);
	void ReGenerate();
	Sphere* Clone() const override;

	#pragma region Getter
	inline const auto& Get_Count() const { return m_Count; }
	inline const auto& Get_Radius() const { return m_Radius; }
	#pragma endregion

	#pragma region Reference
	inline auto& Ref_Count() { return m_Count; }
	inline auto& Ref_Radius() { return m_Radius; }
	#pragma endregion
private:
	void GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
	GLint m_Count;
	GLfloat m_Radius;
};