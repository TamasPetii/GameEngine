#include "AudioComponent.h"

AudioComponent::AudioComponent() :
	startSound{ false },
	isLooped{ false },
	isPaused{ false },
	speed{ 1.f },
	volume{ 1.f },
	minDist{ 5.f },
	maxDist{ 100.f },
	soundSource{ nullptr },
	sound{ nullptr }
{
}
