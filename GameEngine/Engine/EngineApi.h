#pragma once
#pragma warning(disable: 4251)

#ifdef EXPORT_ENGINE_DLL
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif