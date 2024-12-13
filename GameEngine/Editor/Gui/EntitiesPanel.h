#pragma once
#include <string>
#include <memory>
#include "Scene/Scene.h"
#include "Registry/Registry.h"

#define TITLE_EP(title) (std::string(title) + "##EntitiesPanel").c_str()

class EntitiesPanel
{
private:
	static void Render(std::shared_ptr<Scene> scene);
	static void Update(std::shared_ptr<Scene> scene);
	static void DisplayEntity(std::shared_ptr<Registry> registry, Entity entity);
	static void RenderAddEntityPopUp(std::shared_ptr<Registry> registry);

	static std::vector<Entity> toDelete;
	static std::vector<Entity> toCopy;
	static float regionWidth;
	friend class Gui;
};

