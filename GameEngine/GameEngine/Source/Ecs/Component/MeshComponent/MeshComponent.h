#pragma once
#include <GLM/glm.hpp>

#include "../IComponent.h"
#include "../../../IRenderable.h"
#include "../../../Graphics/MeshSource/MeshSources.h"

namespace Ecs
{
	class MeshComponent : public IComponent
	{
	public:
		MeshComponent();
		MeshComponent(const json& data);
		~MeshComponent();

		json Serialize() const override;
		void DeSerialize(const json& data) override;
		MeshComponent* Clone() const override;

		/*Getter*/
		inline const auto& Get_MeshSource() const { return m_MeshSource; }
		inline const auto& Get_IsHardSurface() const { return m_IsHardSurface; }
		/*Reference*/
		inline auto& Ref_MeshSource() { return m_MeshSource; }
		inline auto& Ref_IsHardSurface() { return m_IsHardSurface; }
	private:
		bool m_IsHardSurface = false;
		IRenderable* m_MeshSource;
	};
}

