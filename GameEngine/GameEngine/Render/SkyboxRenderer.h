#pragma once
#include <memory>
#include "Registry/Registry.h"
#include "Manager/ResourceManager.h"
#include "Manager/TextureManager.h"
#include "Render/OpenGL/Camera.h"

class SkyboxRenderer
{
public:
	static void Render(std::shared_ptr<Registry> registry, std::shared_ptr<Camera> camera);
};

