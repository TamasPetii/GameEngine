#include "PointlightComponent.h"
#include "Render/OpenGL/FrameBufferGL.h"

PointLightComponent::PointLightComponent() :
	toRender{ false },
	color{ 1.f },
	position{ 0, 1, 0 },
	strength{ 1.f },
	weakenDistance{ 3.f },
	useShadow{ false },
	updateFrequency{ 3 },
	shadowSize{ 2048 },
	farPlane{ 10.f },
	frameBuffer{ nullptr }
{
}

PointLightGLSL::PointLightGLSL(const PointLightComponent& component) : 
	color{ component.color, component.strength },
	position{ component.position, component.useShadow ? 1 : 0 },
	viewProj{ component.viewProj[0] , component.viewProj[1], component.viewProj[2], component.viewProj[3], component.viewProj[4], component.viewProj[5] },
	farPlane{ component.farPlane, component.weakenDistance },
	shadowTexture{ component.frameBuffer->GetTextureHandler("depth") }
{
}