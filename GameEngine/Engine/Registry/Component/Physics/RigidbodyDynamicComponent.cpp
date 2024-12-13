#include "RigidbodyDynamicComponent.h"

RigidbodyDynamicComponent::RigidbodyDynamicComponent() :
	mass{ 1.f },
	sFriction{ 1.f },
	dFriction{ 1.f },
	restitution{ 0.1f },
	isKinematic{ false },
	disableGravity{ false },
	lockRotation{ false, false, false },
	lockPosition{ false, false, false },
	shape{ nullptr },
	material{ nullptr },
	dynamicActor{ nullptr }
{
}