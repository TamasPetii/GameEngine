#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <string>
#include "../../../Abstractions/Abstractions.h"

class IMesh
{
public:
	virtual void Render() = 0;
	//virtual void Load(const std::string& path = "") = 0;
};

class MeshBase : public IMesh
{
public:
	MeshBase();
	virtual ~MeshBase();
	void Render() override;
	void UploadBuffersToGpu();
	virtual void LoadIndices() = 0;
	virtual void LoadVertices() = 0;
	static glm::vec3 GenerateNormalVectors(const Vertex& v1, const Vertex& v2, const Vertex& v3);
	std::string name;
protected:
	VertexArrayObject* mVao;
	VertexBufferObject* mVbo;
	IndexBufferObject* mIbo;
	unsigned int mIndexCount;
	unsigned int mVertexCount;
	std::vector<Vertex> mPoints;
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
};

template<typename T>
class Shape : public MeshBase {};