#pragma once
#include "../GameObject.h"

class Cube : public GameObject
{
public:
	Cube();
	~Cube();
	void Render() override;
	inline VertexArrayObject* GetVao() const { return mVao; }
	inline VertexBufferObject* GetVbo() const { return mVbo; }
	inline IndexBufferObject* GetIbo() const { return mIbo; }
private:
	void CreateBuffers();
	void UploadBuffers();
	VertexArrayObject* mVao;
	VertexBufferObject* mVbo;
	IndexBufferObject* mIbo;
	unsigned int mIndexCount;
	unsigned int mVertexCount;
};

