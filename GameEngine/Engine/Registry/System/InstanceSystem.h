#pragma once
#include "EngineApi.h"
#include <algorithm>
#include <execution>
#include <glm/glm.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "System.h"
#include "Registry/Registry.h"
#include "Render/OpenGL/Camera.h"
#include "Manager/ModelManager.h"
#include "Manager/ResourceManager.h"
#include "Registry/Component/TransformComponent.h"
#include "Registry/Component/ShapeComponent.h"
#include "Registry/Component/ModelComponent.h"
#include "Collision/CollisionTester.h"

class ENGINE_API InstanceSystem : public System
{
public:
	static void OnStart(std::shared_ptr<Registry> registry);
	static void OnUpdate(std::shared_ptr<Registry> registry);
private:
	static void GeometryInstances(std::shared_ptr<Registry> registry);
	static void ModelInstances(std::shared_ptr<Registry> registry);
};
