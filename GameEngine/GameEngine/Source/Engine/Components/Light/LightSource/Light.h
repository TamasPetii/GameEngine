#pragma once
#include <GLM/glm.hpp>

class ILight
{
public:
	virtual glm::vec3& GetColor() = 0;
	virtual float& GetDiffuseIntensity() = 0;
	virtual float& GetSpecularIntensity() = 0;
};

class LightBase : public ILight
{
public:
	glm::vec3& GetColor() override { return mColor; }
	float& GetDiffuseIntensity() override { return mDiffuseIntensity; }
	float& GetSpecularIntensity() override { return mSpecularIntensity; }
protected:
	glm::vec3 mColor;
	float mDiffuseIntensity;
	float mSpecularIntensity;
};

template<typename T>
class Light : public LightBase {};