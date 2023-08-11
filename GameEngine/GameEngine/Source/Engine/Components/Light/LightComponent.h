#pragma once
#include "../Component.h"
#include "LightSource/Light.h"

class LightComponent : public Component
{
public:
	void AttachLight(ILight* light) { mLight = light; }
	ILight* GetLightSource() { return mLight; }
private:
	ILight* mLight = nullptr;
};

