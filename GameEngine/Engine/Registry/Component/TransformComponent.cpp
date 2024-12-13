#include "TransformComponent.h"

TransformComponent::TransformComponent() :
	fullTransform{ glm::mat4(1.f) },
	fullTransformIT{ glm::mat4(1.f) },
	modelTransform{ glm::mat4(1.f) },
	modelTransformIT{ glm::mat4(1.f) },
	translateMatrix{ glm::mat4(1.f) },
	rotateMatrix{ glm::mat4(1.f) },
	scaleMatrix{ glm::mat4(1.f) },
	translate{ 0.f, 0.f, 0.f },
	rotate{ 0.f, 0.f, 0.f },
	scale{ 1.f, 1.f, 1.f }
{
}

TransformGLSL::TransformGLSL(const TransformComponent& component) :
	model(component.modelTransform),
	modelIT(component.modelTransformIT)
{}