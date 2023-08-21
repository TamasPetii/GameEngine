#pragma once
#include "../Mesh.h"
#include <fstream>
#include <sstream>
#include <string>

class Model : public MeshBase
{
public:
	static Model* LoadObject(const std::string& path);
private:
	Model() : MeshBase() {}
	void LoadIndices() override {};
	void LoadVertices() override {};
};

