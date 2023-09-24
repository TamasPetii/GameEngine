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

		/*Reference*/
		inline auto& Ref_MeshSource() { return m_MeshSource; }
	private:
		IRenderable* m_MeshSource;
	};
}

