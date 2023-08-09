#include "Shape.h"

Shape::Shape() : GameObject()
{
	mName = "Shape";
	mTexture = nullptr;
	mColor = glm::vec3(1,1,1);
	mUseTexture = false;
	mVao = new VertexArrayObject();
	mIbo = new IndexBufferObject();
	mVbo = new VertexBufferObject();
}

Shape::~Shape()
{
	delete mVao;
	delete mIbo;
	delete mVbo;
}

void Shape::UploadBuffers()
{
	mIndexCount = mIndices.size();
	mVertexCount = mVertices.size();

	mVao->Bind();
	mVbo->AttachData(mVertices, GL_DYNAMIC_DRAW);
	mIbo->AttachData(mIndices, GL_DYNAMIC_DRAW);

	mVao->LinkAttribute(*mVbo, 0, 3, GL_FLOAT, sizeof(Vertex), nullptr);
	mVao->LinkAttribute(*mVbo, 1, 3, GL_FLOAT, sizeof(Vertex), (const void*)(1 * sizeof(glm::vec3)));
	mVao->LinkAttribute(*mVbo, 2, 2, GL_FLOAT, sizeof(Vertex), (const void*)(2 * sizeof(glm::vec3)));

	mVao->UnBind();
	mVbo->UnBind();
	mIbo->UnBind();
}

void Shape::UpdateVertices()
{
	CreateVertices();
	mVbo->AttachData(mVertices, GL_DYNAMIC_DRAW);
	mVertexCount = mVertices.size();
}

void Shape::UpdateIndices()
{
	CreateIndices();
	mIbo->AttachData(mIndices, GL_DYNAMIC_DRAW);
	mIndexCount = mIndices.size();
}

void Shape::UpdateBuffers()
{
	UpdateVertices();
	UpdateIndices();
}

void Shape::Render()
{
	mVao->Bind();
	glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, nullptr);
	mVao->UnBind();
}