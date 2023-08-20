#pragma once
#include "Light.h"

class Directional {};

template<>
class Light<Directional> : public LightBase
{
public:
	Light();
	Light(const glm::vec3& direction);
	glm::vec3& GetDirection() { return mDirection; }
	glm::mat4 GetOrthoMatrix() { return glm::ortho(mShadowBoxX.x, mShadowBoxX.y, mShadowBoxY.x, mShadowBoxY.y, mShadowBoxZ.x, mShadowBoxZ.y); }
	glm::vec2& GetShadowBoxX() { return mShadowBoxX; }
	glm::vec2& GetShadowBoxY() { return mShadowBoxY; }
	glm::vec2& GetShadowBoxZ() { return mShadowBoxZ; }
private:
	glm::vec3 mDirection;

	glm::vec2 mShadowBoxX;
	glm::vec2 mShadowBoxY;
	glm::vec2 mShadowBoxZ;
};

