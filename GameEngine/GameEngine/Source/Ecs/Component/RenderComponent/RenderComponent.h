#pragma once
#pragma once
#include "../IComponent.h"
#include "../../../Graphics/Texture/ImageTexture.h"
#include <GLM/glm.hpp>

namespace Ecs
{
	struct MaterialProperties
	{
		glm::vec3 ambient = glm::vec3(1);
		glm::vec3 diffuse = glm::vec3(1);;
		glm::vec3 specular = glm::vec3(1);;
	};

	struct TextureProperties
	{
		glm::vec3 scale;
		ImageTexture* diffuse = nullptr;
		ImageTexture* specular = nullptr;
		ImageTexture* normal = nullptr;
		ImageTexture* displacement = nullptr;
	};

	class RenderComponent : public IComponent
	{
	public:
		RenderComponent();
		RenderComponent(const json& data);

		json Serialize() const override;
		void DeSerialize(const json& data) override;
		RenderComponent* Clone() const override;

		/*Getter*/
		inline const auto& Get_IsVisible() const { return m_IsVisible; }
		inline const auto& Get_Material() const { return m_Material; }
		inline const auto& Get_Texture() const { return m_Texture; }

		/*Reference*/
		inline auto& Ref_IsVisible() { return m_IsVisible; }
		inline auto& Ref_Material() { return m_Material; }
		inline auto& Ref_Texture() { return m_Texture; }
	private:
		bool m_IsVisible;
		bool m_IsLightCaster;
		bool m_IsShadowCaster;
		MaterialProperties m_Material;
		TextureProperties m_Texture;
	};
}



