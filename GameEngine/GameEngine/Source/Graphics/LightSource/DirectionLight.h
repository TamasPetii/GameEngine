#pragma once
#include "Light.h"

class DirectionLight : public Light
{
public:
	json Serialize() const override;
	void DeSerialize(const json& data) override;
	DirectionLight* Clone() const override;

	DirectionLight();
	DirectionLight(const json& data);
	DirectionLight(const glm::vec3& direction);

	/* <<Getter>> */
	inline const auto& Get_Direction() const { return m_Direction; }
	inline const auto& Get_ShadowBoxX() const { return m_ShadowBoxX; }
	inline const auto& Get_ShadowBoxY() const { return m_ShadowBoxY; }
	inline const auto& Get_ShadowBoxZ() const { return m_ShadowBoxZ; }
	inline const auto  Get_OrthoMatrix() const { return glm::ortho(m_ShadowBoxX.x, m_ShadowBoxX.y, m_ShadowBoxY.x, m_ShadowBoxY.y, m_ShadowBoxZ.x, m_ShadowBoxZ.y); }
	/* <<Reference>> */
	inline auto& Ref_Direction() { return m_Direction; }
	inline auto& Ref_ShadowBoxX() { return m_ShadowBoxX; }
	inline auto& Ref_ShadowBoxY() { return m_ShadowBoxY; }
	inline auto& Ref_ShadowBoxZ() { return m_ShadowBoxZ; }
private:
	glm::vec3 m_Direction;
	glm::vec2 m_ShadowBoxX;
	glm::vec2 m_ShadowBoxY;
	glm::vec2 m_ShadowBoxZ;
};

