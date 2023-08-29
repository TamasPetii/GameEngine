#include "ModelPart.h"

ModelPart::ModelPart(const std::vector<Vertex>& vertices, const std::vector<GLuint> indices, const ModelMaterial& material, int num)
{
	this->num = num;
	mMaterial = material;
	mVertices = std::move(vertices);
	mIndices = std::move(indices);
	UploadBuffersToGpu();
}

void ModelPart::RenderPart(IFrameBufferObject* frameBuffer, Program* shaderProgram, const glm::mat4& transform)
{
	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_M", transform);
	shaderProgram->SetUniform("u_MIT", glm::transpose(glm::inverse(transform)));
	shaderProgram->SetUniform("u_Material.ambient", mMaterial.ambient);
	shaderProgram->SetUniform("u_Material.diffuse", mMaterial.diffuse);
	shaderProgram->SetUniform("u_Material.specular", mMaterial.specular);
	this->Render();
}