#pragma once
#pragma warning(disable: 4251)
#define _USE_MATH_DEFINES 

#ifdef EXPORT_SCRIPT_DLL
#define SCRIPT_API __declspec(dllexport)
#else
#define SCRIPT_API __declspec(dllimport)
#endif

#include "Registry/Registry.h"
#include "Registry/Component/Script.h"
#include "Registry/Component/Components.h"