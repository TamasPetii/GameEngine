#pragma once
#include "../Component.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform2.hpp>

class TransformComponent : public Component
{
public:
	json SaveToJson() const override;
	void LoadFromJson(const json& object) override;
	
	TransformComponent();
	TransformComponent(const json& object);
	TransformComponent(const TransformComponent& other) = default;
	~TransformComponent() = default;
	TransformComponent* Clone() const;

	/* <<Getter>> */
	const glm::mat4 Get_TransformMatrix() const;
	inline const auto& Get_Translation() const { return m_Translation; }
	inline const auto& Get_Rotation() const { return m_Rotation; }
	inline const auto& Get_Scale() const { return m_Scale; }
	/* <<Reference>> */
	inline auto& Ref_Translation() { return m_Translation; }
	inline auto& Ref_Rotation() { return m_Rotation; }
	inline auto& Ref_Scale() { return m_Scale; }
private:
	//TODO: Rename
	glm::vec3 m_Translation;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
};

