#include "SpotLightComponent.h"
#include "Render/OpenGL/FrameBufferGL.h"

SpotLightComponent::SpotLightComponent() : 
	toRender{ false },
	color{ 1.f },
	position{ 0, 1, 0 },
	direction{ -1 },
	angles{ 5.f, 25.f, 0, 0 },
	length{ 25.f },
	strength{ 1.f },
	updateFrequency{ 2 },
	shadowSize{ 1024 },
	useShadow{ false },
	frameBuffer{nullptr}
{
}

SpotLightComponent::~SpotLightComponent()
{
}

SpotLightGLSL::SpotLightGLSL(const SpotLightComponent& component) :
	color{ component.color, component.strength },
	position{ component.position, component.useShadow ? 1 : 0 },
	direction{ component.direction, component.farPlane },
	viewProj{ component.viewProj },
	angles{ component.angles },
	shadowTexture{ component.frameBuffer->GetTextureHandler("depth") }
{
}