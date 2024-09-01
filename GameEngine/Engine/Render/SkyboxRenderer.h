#pragma once
#include <memory>

#include "EngineApi.h"
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Manager/TextureManager.h"
#include "Render/OpenGL/Camera.h"

class ENGINE_API SkyboxRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry, std::shared_ptr<Camera> camera);
};
