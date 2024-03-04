#pragma once

#define EXPORT_DLL 
#ifdef EXPORT_DLL
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif