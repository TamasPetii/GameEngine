#pragma once
#include "LightSource.h"

class PointLight : public LightSource
{
public:
	PointLight();
	PointLight(const glm::vec3& position);
	~PointLight() = default;
	json SaveToJson() const override { return json(); }
	void LoadFromJson(const json& object) override {}
	/* <<Getter>> */
	inline const auto& Get_Position() const { return m_Position; }
	/* <<Reference>> */
	inline auto& Ref_Position() { return m_Position; }
private:
	glm::vec3 m_Position;
};

