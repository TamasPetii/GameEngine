#include "Simplex.h"

Simplex& Simplex::operator=(const std::vector<glm::vec3>& list)
{
	unsigned int index = 0;

	for (glm::vec3 point : list)
		m_Points[index++] = point;

	m_Size = index;

	return *this;
}

glm::vec3& Simplex::operator[](int i)
{
	return m_Points[i];
}

void Simplex::PushFront(const glm::vec3& point)
{
	m_Points = { point, m_Points[0], m_Points[1], m_Points[2] };
	m_Size = glm::min<unsigned int>(m_Size + 1, 4);
}