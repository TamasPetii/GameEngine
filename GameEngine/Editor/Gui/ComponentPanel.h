#pragma once
#include <string>
#include <GL/glew.h>
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
#include "FileDialogWindows.h"
#include "Scene/Scene.h"
#include <filesystem>
#include "IconsFontAwesome5.h"
#include "Manager/TextureManager.h"
#include "Manager/ModelManager.h"
#include "Settings/GlobalSettings.h"
#include "Manager/PreviewManager.h"
#include "Render/PreviewRenderer.h"
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

	static bool OpenModelAssetPopup;
	static bool OpenShapeAssetPopup;
	static bool OpenTextureAssetPopup;
	static bool OpenAnimationAssetPopup;
	static TextureAssetType textureAssetType;
	static void ShowAssetPopup(std::shared_ptr<Registry> registry);

	friend class Gui;
};

