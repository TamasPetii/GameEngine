#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtx/transform2.hpp>

#include "../../ISerializable.h"
#include "../../IClonable.h"

class Light : public ISerializable, public ICloneable
{
public:
	Light();

	/* <<Getter>> */
	inline const auto& Get_Color() const { return m_Color; }
	inline const auto& Get_CastShadow() const { return m_CastShadow; }
	inline const auto& Get_DiffuseIntensity() const { return m_DiffuseIntensity; }
	inline const auto& Get_SpecularIntensity() const { return m_SpecularIntensity; }
	/* <<Reference>> */
	inline auto& Ref_Color() { return m_Color; }
	inline auto& Ref_CastShadow() { return m_CastShadow; }
	inline auto& Ref_DiffuseIntensity() { return m_DiffuseIntensity; }
	inline auto& Ref_SpecularIntensity() { return m_SpecularIntensity; }
protected:
	glm::vec3 m_Color;
	bool m_CastShadow;
	float m_DiffuseIntensity;
	float m_SpecularIntensity;
};

