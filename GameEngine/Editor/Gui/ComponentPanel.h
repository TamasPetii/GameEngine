#pragma once
#include <string>
#include <memory>
#include "Scene/Scene.h"
#include "Registry/Registry.h"

#define TITLE_CP(title) (std::string(title) + "##ComponentPanel").c_str()

enum class TextureAssetType
{
	DIFFUSE,
	NORMAL,
	SPECULAR,
	DUDV
};

class ComponentPanel
{
private:
	static void Render(std::shared_ptr<Scene> scene);
	static void Update(std::shared_ptr<Scene> scene);

	static void RenderCameraComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderTagComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderTransformComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderMaterialComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderDirlightComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderPointLightComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderSpotLightComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderAddComponentPopUp(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderShapeComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderWaterComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderModelComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderAnimationComponent(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderBoxColliderComponent(std::shared_ptr<Scene> scene, Entity entity);
	static void RenderSphereColliderComponent(std::shared_ptr<Scene> scene, Entity entity);
	static void RenderConvexColliderComponent(std::shared_ptr<Scene> scene, Entity entity);
	static void RenderMeshColliderComponent(std::shared_ptr<Scene> scene, Entity entity);
	static void RenderStaticRigidbodyComponent(std::shared_ptr<Scene> scene, Entity entity);
	static void RenderDynamicRigidbodyComponent(std::shared_ptr<Scene> scene, Entity entity);
	static void RenderScriptComponent(std::shared_ptr<Registry> registry, Entity entity);

	static bool OpenModelAssetPopup;
	static bool OpenShapeAssetPopup;
	static bool OpenTextureAssetPopup;
	static bool OpenAnimationAssetPopup;
	static TextureAssetType textureAssetType;
	static void ShowAssetPopup(std::shared_ptr<Registry> registry);

	friend class Gui;
};

