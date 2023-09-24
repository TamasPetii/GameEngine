#pragma once
#include "../IComponent.h"

class Script;
class Entity;

namespace Ecs
{
	class ScriptComponent : public IComponent
	{
	public:
		ScriptComponent();
		ScriptComponent(const json& data);
		~ScriptComponent();

		json Serialize() const override;
		void DeSerialize(const json& data) override;
		ScriptComponent* Clone() const override;

		/* <<Getter>>*/
		inline const auto& Get_Scripts() const { return m_Scripts; }
		/* <<Reference>> */
		inline auto& Ref_Scripts() { return m_Scripts; }
	private:
		std::unordered_map<std::string, Script*> m_Scripts;
	};
}


