#include "SkyboxComponent.h"

SkyComponent* SkyComponent::Clone() const
{
	return new SkyComponent(*this);
}