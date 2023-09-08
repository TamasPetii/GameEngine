#pragma once
#include "../Component.h"
#include "LightSource/LightSource.h"
#include "LightSource/PointLight.h"
#include "LightSource/DirectionLight.h"

class LightComponent : public Component
{
public:
	json SaveToJson() const override;
	void LoadFromJson(const json& object) override;

	LightComponent();
	LightComponent(const json& object);
	LightComponent(const LightComponent& other);
	~LightComponent();

	LightComponent* Clone() const;

	inline void AttachLight(LightSource* light) { m_LightSource = light; }

	/*Getter*/
	inline const auto& Get_LightSource() const { return m_LightSource; }
	/*Reference*/
	inline auto& Ref_LightSource() { return m_LightSource; }
private:
	LightSource* m_LightSource;
};

