#pragma once
#include <glm/glm.hpp>
#include <array>
#include <vector>

class Simplex
{
public:
	Simplex()
		: m_Size(0)
	{
	}

	Simplex& operator=(const std::vector<glm::vec3>& list)
	{
		unsigned int index = 0;

		for (glm::vec3 point : list)
			m_Points[index++] = point;

		m_Size = index;

		return *this;
	}

	inline glm::vec3& operator[](int i)
	{
		return m_Points[i];
	}

	inline void PushFront(const glm::vec3& point)
	{
		m_Points = { point, m_Points[0], m_Points[1], m_Points[2] };
		m_Size = glm::min<unsigned int>(m_Size + 1, 4);
	}

	int GetSize() const { return m_Size; }
private:
	int m_Size;
	std::array<glm::vec3, 4> m_Points;
};