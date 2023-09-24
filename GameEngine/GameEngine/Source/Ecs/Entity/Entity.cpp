#include "Entity.h"

namespace Ecs
{
	Entity::Entity()
	{
		static std::random_device rnd;
		static std::mt19937 gen(rnd());
		static std::uniform_int_distribution<unsigned int> dist;

		do
		{
			m_Id = dist(gen);
		}
		while (m_Id == 0);
	}
}