#include "RigidbodyStaticComponent.h"

RigidbodyStaticComponent::RigidbodyStaticComponent() :
	sFriction{ 1.f },
	dFriction{ 1.f },
	restitution{ 0.1f },
	shape{ nullptr },
	material{ nullptr },
	staticActor{ nullptr }
{
}
