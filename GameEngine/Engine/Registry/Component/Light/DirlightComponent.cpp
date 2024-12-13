#include "DirlightComponent.h"
#include "Render/OpenGL/FrameBufferGL.h"

DirlightComponent::DirlightComponent() :
	color{ 1.f },
	strength{ 0.5f },
	direction{ -1.f },
	useShadow{ false },
	updateFrequency{ 1 },
	shadowSize{ 2048 },
	farPlane{ 0.1f, 25.f, 70.f, 135.f, 250.f },
	frameBuffer{ nullptr }
{
}

DirlightComponent::~DirlightComponent()
{
}

DirlightGLSL::DirlightGLSL(const DirlightComponent& component) : 
	color{ component.color, component.strength },
	direction{ component.direction, component.useShadow ? 1 : 0 },
	farPlane{ component.farPlane[0], component.farPlane[1], component.farPlane[2], component.farPlane[3] },
	viewProj{ component.viewProj[0], component.viewProj[1], component.viewProj[2], component.viewProj[3] },
	shadowTexture{ component.frameBuffer->GetTextureHandler("depth") },
	filler{0}
{
}

DirlightLineGLSL::DirlightLineGLSL(const glm::vec4& position, const glm::vec4& direction) :
	position{ position },
	direction{ direction }
{
}