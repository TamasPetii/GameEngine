#pragma once
#include "LightSource.h"

class PointLight : public LightSource
{
public:
	json SaveToJson() const override;
	void LoadFromJson(const json& object) override;

	PointLight();
	PointLight(const glm::vec3& position);
	PointLight(const json& object);
	PointLight(const PointLight& other) = default;
	~PointLight() = default;

	PointLight* Clone() const override;

	/* <<Getter>> */
	inline const auto& Get_Position() const { return m_Position; }
	/* <<Reference>> */
	inline auto& Ref_Position() { return m_Position; }
private:
	glm::vec3 m_Position;
};

