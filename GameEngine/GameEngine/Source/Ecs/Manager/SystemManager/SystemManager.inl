#include "SystemManager.h"

template<typename T>
T* SystemManager::GetSystem()
{
	auto it = m_SystemMap.find(typeid(T));

	if (it != m_SystemMap.end())
	{
		return dynamic_cast<T*>(it->second);
	}

	std::runtime_error("No such kind of System type in SystemManager");
}