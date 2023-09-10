#pragma once
#include "../Component.h"
#include "../../../Renderer/Utility/ImageTexture.h"
#include "MeshSource/Renderable.h"
#include "MeshSource/Mesh.h"
#include "MeshSource/Shape3D/Sphere.h"
#include "MeshSource/Shape3D/Cube.h"
#include "MeshSource/Shape3D/Torus.h"
#include "MeshSource/Shape3D/Cylinder.h"
#include "MeshSource/Shape2D/Plane.h"

struct MaterialProperties
{
	glm::vec3 ambient = glm::vec3(1);
	glm::vec3 diffuse = glm::vec3(1);;
	glm::vec3 specular = glm::vec3(1);;
};

struct TextureProperties
{
	float scale = 1;
	float scaleX = 1;
	float scaleY = 1;
	ImageTexture* texture = nullptr;
	ImageTexture* normal = nullptr;
	ImageTexture* height = nullptr;
};

class MeshComponent : public Component
{
public:
	json SaveToJson() const override;
	void LoadFromJson(const json& object) override;

	MeshComponent();
	MeshComponent(const json& object);
	MeshComponent(const MeshComponent& other);
	~MeshComponent();

	MeshComponent* Clone() const;

	void Render();
	inline void AttachRenderable(Renderable* renderable) { m_RenderObject = renderable; }

	/*Getter*/
	inline const auto& Get_Renderable() const { return m_RenderObject; }
	inline const auto& Get_Material() const { return m_Material; }
	inline const auto& Get_Textures() const { return m_Textures; }
	inline const auto& Get_HardSurface() const { return m_HardSurface; }

	/*Reference*/
	inline auto& Ref_Renderable() { return m_RenderObject; }
	inline auto& Ref_Material() { return m_Material; }
	inline auto& Ref_Textures() { return m_Textures; }
	inline auto& Ref_HardSurface() { return m_HardSurface; }
private:
	bool m_HardSurface;
	Renderable* m_RenderObject;
	MaterialProperties m_Material;
	TextureProperties m_Textures;
};

