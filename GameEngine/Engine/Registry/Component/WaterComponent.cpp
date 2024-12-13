#include "WaterComponent.h"
#include "Render/OpenGL/FramebufferGL.h"
#include "Render/OpenGL/TextureGL.h"

WaterComponent::WaterComponent() :
	updateFrequency{ 1 },
	size{ 1024 },
	plane{ 0.f, 1.f, 0.f, 0.f },
	dudvScale{ 1.f, 1.f },
	dudvWaveStrength{ 0.025f, 0.025f },
	dudvMoveFactor{ 0.f },
	dudvMoveSpeed{ 0.030f },
	dudv{ nullptr },
	reflectionFbo{ nullptr },
	refractionFbo{ nullptr }
{
}

WaterComponent::~WaterComponent()
{
}

WaterGLSL::WaterGLSL(const WaterComponent& component) :
	plane{ component.plane },
	scale{ glm::vec4(component.dudvScale, component.dudvWaveStrength) },
	move{ glm::vec2(component.dudvMoveFactor, component.dudvMoveSpeed) },
	dudv{ component.dudv != nullptr ? component.dudv->GetTextureHandler() : 0 }
{
}
