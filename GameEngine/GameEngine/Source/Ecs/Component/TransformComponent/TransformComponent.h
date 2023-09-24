#pragma once
#include "../IComponent.h"
#include <GLM/glm.hpp>

namespace Ecs
{
	class TransformComponent : public IComponent
	{
	public:
		TransformComponent();
		TransformComponent(const json& data);

		json Serialize() const override;
		void DeSerialize(const json& data) override;
		TransformComponent* Clone() const override;

		/* <<Getter>>*/
		inline const auto& Get_Translation() const { return m_Translation; }
		inline const auto& Get_Rotation() const { return m_Rotation; }
		inline const auto& Get_Scale() const { return m_Scale; }
		/* <<Reference>> */
		inline auto& Ref_Translation() { return m_Translation; }
		inline auto& Ref_Rotation() { return m_Rotation; }
		inline auto& Ref_Scale() { return m_Scale; }
	private:
		glm::vec3 m_Translation;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
	};
}


