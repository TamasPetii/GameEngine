#include "ShapeComponent.h"
#include "Render/Geometry/Geometry.h"

ShapeComponent::ShapeComponent() :
	lodLevel{ 0 },
	automaticLod{ false },
	toRender { true },
	castShadow{ true },
	isInstanced{ false },
	receiveShadow{ true },
	useFrustumCulling{ true },
	shape{ nullptr }
{
}

ShapeGLSL::ShapeGLSL(const ShapeComponent& component) : 
	receiveShadow{ component.receiveShadow }
{
	
}