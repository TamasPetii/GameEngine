#include "AnimationComponent.h"
#include "Animation/Animation.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"

AnimationComponent::AnimationComponent() :
	lockToFps{ false },
	time{ 0.f },
	speed{ 1.f },
	animation{ nullptr },
	boneTransformSsbo{ nullptr }
{
}

AnimationComponent::~AnimationComponent()
{
}