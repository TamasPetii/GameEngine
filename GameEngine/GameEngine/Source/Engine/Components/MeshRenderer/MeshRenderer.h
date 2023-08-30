#pragma once
#include "../Component.h"
#include "../../Abstractions/Abstractions.h"

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct Texture
{
	ImageTexture* diffuse;
	ImageTexture* normal;
	ImageTexture* height;
};

enum class MeshRenderType
{
	VERTEX_DRAW,
	INDEX_DRAW,
	INSTANCE_DRAW
};

class MeshRenderer : public Component
{
public:
	json SaveToJson() const override { return json(); }
	void LoadFromJson(const json& object) override {}

	MeshRenderer();
	~MeshRenderer();

	/* <<Getter>> */
	inline const auto& Get_Material() const { return m_Material; }
	inline const auto& Get_Texture() const { return m_Texture; }
	/* <<Reference>> */
	inline auto& Ref_Material() const { return m_Material; }
	inline auto& Ref_Texture() const { return m_Texture; }
	/* <<Setter>> */
	inline void Set_Material(const Material& material) { m_Material = material; }
	inline void Set_Texture(const Texture& texture) { m_Texture = texture; }
private:
	bool m_CastLight;
	bool m_CastShadow;
	Material m_Material;
	Texture  m_Texture;
	MeshRenderType m_MeshRenderType;
};

