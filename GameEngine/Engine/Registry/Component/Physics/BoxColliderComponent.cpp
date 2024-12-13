#include "BoxColliderComponent.h"

BoxColliderComponent::BoxColliderComponent() :
	isTrigger{ false },
	calculateAutomatic{ true },
	bindToTransformTranslate{ true },
	bindToTransformScale{ true },
	halfExtents{ 1.f, 1.f, 1.f },
	origin{ 0.f, 0.f, 0.f },
	transformedExtents{ 0.f, 0.f, 0.f },
	transformedOrigin{ 0.f, 0.f, 0.f }
{
}