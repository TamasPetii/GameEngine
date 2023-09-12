#include "ModelPart.h"

ModelPart::ModelPart(const std::vector<Vertex>& vertex, const std::vector<GLuint>& indices, const Material& material, const Textures& texture)
{
	m_Material = material;
	m_Textures = texture;
	UploadToGpu(vertex, indices);
}