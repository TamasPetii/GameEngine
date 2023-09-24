#pragma once
#include "../Shape.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <unordered_set>


class Sphere : public Shape
{
public:
	Sphere();
	Sphere(GLint count, GLfloat radius);
	Sphere(const json& data);

	json Serialize() const override;
	void DeSerialize(const json& data) override;
	Sphere* Clone() const override;

	void GenerateVertices() override;
	void GenerateIndices() override;
	void GenerateNormals() override;

	inline const auto& Get_Count() const { return m_Count; }
	inline const auto& Get_Radius() const { return m_Radius; }

	inline auto& Ref_Count() { return m_Count; }
	inline auto& Ref_Radius() { return m_Radius; }
private:
	GLint m_Count;
	GLfloat m_Radius;
};