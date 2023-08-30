#pragma once
#include "../Component.h"
#include "LightSource/LightSource.h"

class LightComponent : public Component
{
public:
	LightComponent();
	~LightComponent();
	void AttachLight(LightSource* lightSource) { m_LightSource = lightSource; }
	/* <<Getter>> */
	inline const auto& Get_LightSource() const { return m_LightSource; }
	/* <<Reference>> */
	inline auto& Ref_LightSource() { return m_LightSource; }
private:
	LightSource* m_LightSource;
};

