#include "Mesh.h"

MeshBase::MeshBase()
{
	mVao = new VertexArrayObject();
	mIbo = new IndexBufferObject();
	mVbo = new VertexBufferObject();
}

MeshBase::~MeshBase()
{
	delete mVao;
	delete mIbo;
	delete mVbo;
}

void MeshBase::Render()
{
	mVao->Bind();
	glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, nullptr);
	mVao->UnBind();
}

void MeshBase::UploadBuffersToGpu()
{

	mIndexCount = mIndices.size();
	mVertexCount = mVertices.size();
	std::cout << mIndexCount << " " << mVertexCount << std::endl;

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

glm::vec3 MeshBase::GenerateNormalVectors(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	glm::vec3 vector1 = v3.mPosition - v2.mPosition;
	glm::vec3 vector2 = v1.mPosition - v2.mPosition;
	return glm::normalize(glm::cross(vector1, vector2));
}
