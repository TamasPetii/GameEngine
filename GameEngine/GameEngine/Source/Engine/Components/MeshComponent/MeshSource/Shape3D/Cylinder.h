#pragma once
#include "../Shape.h"

class Cylinder : public Shape
{
public:
	Cylinder();
	void ReGenerate();

	#pragma region Getter
		inline const auto& Get_Count() const { return m_Count; }
		inline const auto& Get_RadiusTop() const { return m_RadiusTop; }
		inline const auto& Get_RadiusBottom() const { return m_RadiusBottom; }
		inline const auto& Get_Height() const { return m_Height; }
	#pragma endregion

	#pragma region Reference
		inline auto& Ref_Count() { return m_Count; }
		inline auto& Ref_RadiusTop() { return m_RadiusTop; }
		inline auto& Ref_RadiusBottom() { return m_RadiusBottom; }
		inline auto& Ref_Height() { return m_Height; }
	#pragma endregion
private:
	void GenerateShape(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
	void GenerateShapeAverage(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
	GLint m_Count;
	GLfloat m_Height;
	GLfloat m_RadiusTop;
	GLfloat m_RadiusBottom;
};

