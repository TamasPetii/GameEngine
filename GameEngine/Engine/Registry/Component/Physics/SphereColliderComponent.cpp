#include "SphereColliderComponent.h"

SphereColliderComponent::SphereColliderComponent() :
	isTrigger{ false },
	calculateAutomatic{ true },
	bindToTransformTranslate{ true },
	bindToTransformScale{ true },
	radius{ 1.f },
	origin{ 0.f, 0.f, 0.f },
	transformedRadius{ 0.f },
	transformedOrigin{ 0.f, 0.f, 0.f }
{
}
