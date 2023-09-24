#pragma once
#include "../Shape.h"

class Torus : public Shape
{
public:
	Torus();
	Torus(GLint count, GLfloat radiusPrimary, GLfloat radiusSecondary);
	Torus(const json& data);

	json Serialize() const override;
	void DeSerialize(const json& data) override;
	Torus* Clone() const override;

	void GenerateVertices() override;
	void GenerateIndices() override;
	void GenerateNormals() override;

	inline const auto& Get_Count() const { return m_Count; }
	inline const auto& Get_RadiusPrimary() const { return m_RadiusPrimary; }
	inline const auto& Get_RadiusSecondary() const { return m_RadiusSecondary; }

	inline auto& Ref_Count() { return m_Count; }
	inline auto& Ref_RadiusPrimary() { return m_RadiusPrimary; }
	inline auto& Ref_RadiusSecondary() { return m_RadiusSecondary; }
private:
	GLuint m_Count;
	GLfloat m_RadiusPrimary;
	GLfloat m_RadiusSecondary;
};