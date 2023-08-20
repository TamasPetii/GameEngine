#pragma once
#include "../Component.h"
#include "LightSource/Light.h"

class LightComponent : public Component
{
public:
	~LightComponent();
	void AttachLight(ILight* light) { mLight = light; }
	ILight* GetLightSource() { return mLight; }
	bool& GetUseShadow() { return mUseShadow; }
private:
	bool mUseShadow = false;
	ILight* mLight = nullptr;
};

