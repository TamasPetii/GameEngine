#pragma once
#include "../IComponent.h"

namespace Ecs
{
	class TagComponent : public IComponent
	{
	public:
		TagComponent();
		TagComponent(const json& data);

		json Serialize() const override;
		void DeSerialize(const json& data) override;
		TagComponent* Clone() const override;

		/* <<Getter>>*/
		inline const auto& Get_Tag() const { return m_Tag; }
		/* <<Reference>> */
		inline auto& Ref_Tag() { return m_Tag; }
	private:
		std::string m_Tag;
	};
}


