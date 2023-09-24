#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include "Mesh.h"
#include <tuple>
#include <string>
#include <type_traits>
#include <unordered_set>

class Shape : public Mesh
{
public:
	Shape() {}
protected:
	virtual void GenerateShape();
	virtual void ReGenerateShape();
	virtual void GenerateVertices() = 0;
	virtual void GenerateIndices() = 0;
	virtual void GenerateNormals() = 0;
};