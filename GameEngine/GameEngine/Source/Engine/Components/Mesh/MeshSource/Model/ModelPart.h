#pragma once
#include "../Mesh.h"

struct ModelMaterial
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class ModelPart : public MeshBase
{
public:
	ModelPart(const std::vector<Vertex>& vertices, const std::vector<GLuint> indices, const ModelMaterial& material, int num);
	void RenderPart(IFrameBufferObject* frameBuffer, Program* shaderProgram, const glm::mat4& transform);
private:
	void LoadIndices() override {};
	void LoadVertices() override {};
	ModelMaterial mMaterial;
	int num;
};

