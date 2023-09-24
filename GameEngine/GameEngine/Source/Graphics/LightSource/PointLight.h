#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	json Serialize() const override;
	void DeSerialize(const json& data) override;
	PointLight* Clone() const override;

	PointLight();
	PointLight(const json& data);
	PointLight(const glm::vec3& position);

	/* <<Getter>> */
	inline const auto& Get_Position() const { return m_Position; }
	/* <<Reference>> */
	inline auto& Ref_Position() { return m_Position; }
private:
	glm::vec3 m_Position;
};

