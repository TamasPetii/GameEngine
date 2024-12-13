#include "MaterialComponent.h"
#include "Render/OpenGL/TextureGL.h"

MaterialComponent::MaterialComponent() : 
	useBloom{ false },
	color{ 1.f },
	shinniness{ 0.25f },
	textureScale{ 1.f, 1.f },
	diffuse{ nullptr },
	normal{ nullptr },
	specular{ nullptr }
{
}

MaterialGLSL::MaterialGLSL(const MaterialComponent& component) : 
	color{ component.color },
	shinniness{ component.shinniness, component.useBloom ? 1 : 0, component.textureScale.x, component.textureScale.y },
	diffuse{ component.diffuse ? component.diffuse->GetTextureHandler() : 0 },
	normal{ component.normal ? component.normal->GetTextureHandler() : 0 },
	specular{ component.specular ? component.specular->GetTextureHandler() : 0 },
	filler{ 0 }
{
}