#pragma once
#include "../GameObject.h"

class Light : public GameObject
{
public:
	Light();

	//Getter
	inline glm::vec3& GetColorRef() { return mColor; }
	inline float& GetDiffuseRef() { return mDiffuse; }
	inline float& GetSpecularRef() { return mSpecular; }
	inline float& GetVolumeRef() { return mVolume; }
protected:
	glm::vec3 mColor;
	float mDiffuse;
	float mSpecular;
	float mVolume;
};

