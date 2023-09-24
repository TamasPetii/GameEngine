#pragma once
#include <iostream>
#include <typeindex>
#include <unordered_map>

#include "../IManager.h"
#include "../../System/ISystem.h"

class SystemManager : public IManager
{
public:
	template<typename T>
	T* GetSystem();
private:
	std::unordered_map<std::type_index, ISystem*> m_SystemMap;
};

#include "SystemManager.inl"
