#pragma once
#include "../IComponent.h"
#include "../../../Graphics/Texture/ImageTexture.h"

namespace Ecs
{
	enum SkyboxType
	{
		BOX, 
		SPHERE
	};

	class SkyboxComponent : public IComponent
	{
	public: 
		SkyboxComponent();
		SkyboxComponent(const json& data);

		json Serialize() const override;
		void DeSerialize(const json& data) override;
		SkyboxComponent* Clone() const override;
	private:
		SkyboxType m_SkyboxType;
		ImageTexture* m_SkyboxTexture;
	};
};


