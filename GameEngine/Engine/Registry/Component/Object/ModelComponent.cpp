#include "ModelComponent.h"
#include "Model/Model.h"

ModelComponent::ModelComponent() :
	lodLevel{ 0 },
	automaticLod{ true },
	toRender{ true },
	castShadow{ true },
	isInstanced{ false },
	receiveShadow{ true },
	useFrustumCulling{ true },
	model{ nullptr }
{
}

ModelComponent::~ModelComponent()
{
}

ModelGLSL::ModelGLSL(const ModelComponent& component) : 
	receiveShadow{ component .receiveShadow }
{
}