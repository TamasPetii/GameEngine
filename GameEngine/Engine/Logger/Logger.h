#pragma once
#include "EngineApi.h"
#include <sstream>

enum LOG
{
	INIT,
	TIME,
	RENDER
};

template<LOG T>
class ENGINE_API Logger
{
public:
	static std::stringstream& Log() { return sstream; }
	static std::string GetLog() { return sstream.str(); }
private:
	static std::stringstream sstream;
};

template<LOG T>
inline std::stringstream Logger<T>::sstream;