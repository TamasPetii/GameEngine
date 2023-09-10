#pragma once
#include "../Component.h"
#include "../../../Renderer/Utility/ImageTexture.h"

enum SkyType
{
	SkyBox,
	SkySphere
};

class SkyComponent : public Component
{
public:
	json SaveToJson() const override { return json(); }
	void LoadFromJson(const json& object) override {}

	SkyComponent* Clone() const;

	inline auto& Get_SkyType() { return mSkyType; }
	inline auto& Get_SkyTexture() { return mSkyTexture; }
private:
	SkyType mSkyType;
	ImageTexture* mSkyTexture;
};