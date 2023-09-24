#pragma once
#include "../IComponent.h"
#include "../../../Graphics/LightSource/LightSources.h"

namespace Ecs
{
	class LightComponent : public IComponent
	{
	public:
		json Serialize() const override;
		void DeSerialize(const json& data) override;
		LightComponent* Clone() const override;

		LightComponent();
		LightComponent(const json& object);
		~LightComponent();

		/*Getter*/
		inline const auto& Get_LightSource() const { return m_LightSource; }
		/*Reference*/
		inline auto& Ref_LightSource() { return m_LightSource; }
	private:
		Light* m_LightSource;
	};
}


