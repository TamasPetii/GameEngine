#pragma once
#include "../GameObject.h"

class Shape : public GameObject
{
public:
	Shape();
	virtual ~Shape();
	void Render();
	virtual void CreateVertices() = 0;
	virtual void CreateIndices() = 0;
	void UpdateVertices();
	void UpdateIndices();
	void UpdateBuffers();
	inline unsigned int GetIndexCount() const { return mIndexCount; }
	inline unsigned int GetVertexCount() const { return mVertexCount; }
	inline VertexArrayObject* GetVao() { return mVao; }
	inline VertexBufferObject* GetVbo() { return mVbo; }
	inline IndexBufferObject* GetIbo() { return mIbo; }
	inline Texture2D* GetTexture() { return mTexture; }
	inline glm::vec3& GetColorRef() { return mColor; }
	inline bool& GetUseTextureRef() { return mUseTexture; }
	inline void SetTexture(Texture2D* texture) { mTexture = texture; }
protected:
	void UploadBuffers();
	VertexArrayObject* mVao;
	VertexBufferObject* mVbo;
	IndexBufferObject* mIbo;
	std::vector<Vertex> mVertices;
	std::vector<unsigned int> mIndices;
	unsigned int mIndexCount;
	unsigned int mVertexCount;
	Texture2D* mTexture;
	glm::vec3 mColor;
	bool mUseTexture;
};

