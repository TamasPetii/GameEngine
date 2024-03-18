#pragma once
#include "EngineApi.h"
#include <glm/glm.hpp>
#include <array>
#include <vector>

class ENGINE_API Simplex
{
public:
	Simplex()
		: m_Size(0)
	{
	}

	Simplex& operator=(const std::vector<glm::vec3>& list);
	glm::vec3& operator[](int i);
	void PushFront(const glm::vec3& point);
	int GetSize() const { return m_Size; }
private:
	int m_Size;
	std::array<glm::vec3, 4> m_Points;
};