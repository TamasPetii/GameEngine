#include "EntitiesPanel.h"

void EntitiesPanel::Update(std::shared_ptr<Scene> scene)
{

}

void EntitiesPanel::Render(std::shared_ptr<Scene> scene)
{
	if (GlobalSettings::GameViewActive)
		return;

	if (ImGui::Begin(TITLE_EP("Entities")))
	{
		auto registry = scene->GetRegistry();

		for (const auto& entity : registry->GetActiveEntities())
		{
			regionWidth = ImGui::GetContentRegionAvail().x;

			if(!registry->HasParent(entity))
				DisplayEntity(registry, entity);
		}

		RenderAddEntityPopUp(registry);
	}

	ImGui::End();
}

void EntitiesPanel::DisplayEntity(std::shared_ptr<Registry> registry, Entity entity)
{
	std::string name = "Entity";
	if (registry->HasComponent<TagComponent>(entity))
		name = registry->GetComponent<TagComponent>(entity).name;
	bool open = ImGui::TreeNodeEx(TITLE_EP(std::string(ICON_FA_CUBE) + " " + name + "##" + std::to_string(entity)));

	if (ImGui::IsItemClicked())
	{
		registry->SetActiveEntity(entity);
	}

	if (ImGui::BeginDragDropSource())
	{
		ImGui::SetDragDropPayload("Entity_Tree", &entity, sizeof(Entity));
		ImGui::Text("Entity | %d", entity);
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity_Tree"))
		{
			Entity droppedEntity = *(Entity*)payload->Data;

			if (droppedEntity != null && !registry->IsDeepConnected(entity, droppedEntity) && !registry->IsDeepConnected(droppedEntity, entity))
			{
				registry->SetParent(droppedEntity, entity);
				std::cout << "Dropped Entity = " << droppedEntity << std::endl;
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	ImGui::SetCursorPosX(regionWidth - 40);

	std::string eye = ICON_FA_EYE;
	if (registry->HasComponent<ShapeComponent>(entity) && !registry->GetComponent<ShapeComponent>(entity).toRender)
		eye = ICON_FA_EYE_SLASH;
	else if(registry->HasComponent<ModelComponent>(entity) && !registry->GetComponent<ModelComponent>(entity).toRender)
		eye = ICON_FA_EYE_SLASH;

	ImGui::Text("%s", eye);
	ImGui::SameLine();
	ImGui::Text(" %d", entity);


	if (open)
	{
		for (auto child : registry->GetChildren()[entity])
			DisplayEntity(registry, child);

		ImGui::TreePop();
	}
}

void EntitiesPanel::RenderAddEntityPopUp(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto shapeIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Shapes.png");

    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::MenuItem(TITLE_EP(std::string(ICON_FA_CUBE) + " Empty")))
        {
            auto entity = registry->CreateEntity();
            registry->SetActiveEntity(entity);
        }

		if (ImGui::MenuItem(TITLE_EP(std::string(ICON_FA_VIDEO) + " Camera")))
		{
			auto entity = registry->CreateEntity();
			registry->SetActiveEntity(entity);
		}

		if (ImGui::BeginMenu(TITLE_EP(std::string(ICON_FA_SHAPES) + " Shapes")))
		{
			if (ImGui::MenuItem(TITLE_EP(std::string(ICON_FA_SQUARE) + " Cube")))
			{
				auto entity = registry->CreateEntity();
				registry->AddComponent<TagComponent>(entity);
				registry->AddComponent<TransformComponent>(entity);
				registry->AddComponent<MaterialComponent>(entity);
				registry->AddComponent<ShapeComponent>(entity);
				registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cube");
				registry->GetComponent<TagComponent>(entity).name = "Cube";
				registry->SetActiveEntity(entity);
			}

			if (ImGui::MenuItem(TITLE_EP(std::string(ICON_FA_CIRCLE) + " Sphere")))
			{
				auto entity = registry->CreateEntity();
				registry->AddComponent<TagComponent>(entity);
				registry->AddComponent<TransformComponent>(entity);
				registry->AddComponent<MaterialComponent>(entity);
				registry->AddComponent<ShapeComponent>(entity);
				registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Sphere");
				registry->GetComponent<TagComponent>(entity).name = "Sphere";
				registry->SetActiveEntity(entity);
			}

			if (ImGui::MenuItem(TITLE_EP(std::string(ICON_FA_SHAPES) + " Cylinder")))
			{
				auto entity = registry->CreateEntity();
				registry->AddComponent<TagComponent>(entity);
				registry->AddComponent<TransformComponent>(entity);
				registry->AddComponent<MaterialComponent>(entity);
				registry->AddComponent<ShapeComponent>(entity);
				registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cylinder");
				registry->GetComponent<TagComponent>(entity).name = "Cylinder";
				registry->SetActiveEntity(entity);
			}

			if (ImGui::MenuItem(TITLE_EP(std::string(ICON_FA_SHAPES) + " Cone")))
			{
				auto entity = registry->CreateEntity();
				registry->AddComponent<TagComponent>(entity);
				registry->AddComponent<TransformComponent>(entity);
				registry->AddComponent<MaterialComponent>(entity);
				registry->AddComponent<ShapeComponent>(entity);
				registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cone");
				registry->GetComponent<TagComponent>(entity).name = "Cone";
				registry->SetActiveEntity(entity);
			}

			if (ImGui::MenuItem(TITLE_EP(std::string(ICON_FA_SHAPES) + " Pyramid")))
			{
				auto entity = registry->CreateEntity();
				registry->AddComponent<TagComponent>(entity);
				registry->AddComponent<TransformComponent>(entity);
				registry->AddComponent<MaterialComponent>(entity);
				registry->AddComponent<ShapeComponent>(entity);
				registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Pyramid");
				registry->GetComponent<TagComponent>(entity).name = "Pyramid";
				registry->SetActiveEntity(entity);
			}

			if (ImGui::MenuItem(TITLE_EP(std::string(ICON_FA_SHAPES) + " Torus")))
			{
				auto entity = registry->CreateEntity();
				registry->AddComponent<TagComponent>(entity);
				registry->AddComponent<TransformComponent>(entity);
				registry->AddComponent<MaterialComponent>(entity);
				registry->AddComponent<ShapeComponent>(entity);
				registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Torus");
				registry->GetComponent<TagComponent>(entity).name = "Torus";
				registry->SetActiveEntity(entity);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu(TITLE_EP(std::string(ICON_FA_LIGHTBULB) + " Lights")))
		{
			if (ImGui::MenuItem(TITLE_EP(std::string(ICON_FA_SUN) + " Direction")))
			{
				auto entity = registry->CreateEntity();
				registry->AddComponent<TagComponent>(entity);
				registry->AddComponent<TransformComponent>(entity);
				registry->AddComponent<DirlightComponent>(entity);
				registry->GetComponent<TagComponent>(entity).name = "Direction Light";
				registry->SetActiveEntity(entity);
			}

			if (ImGui::MenuItem(TITLE_EP(std::string(ICON_FA_LIGHTBULB) + " Point")))
			{
				auto entity = registry->CreateEntity();
				registry->AddComponent<TagComponent>(entity);
				registry->AddComponent<TransformComponent>(entity);
				registry->AddComponent<PointLightComponent>(entity);
				registry->GetComponent<TagComponent>(entity).name = "Point Light";
				registry->SetActiveEntity(entity);
			}

			if (ImGui::MenuItem(TITLE_EP(std::string(ICON_FA_LIGHTBULB) + " Spot")))
			{
				auto entity = registry->CreateEntity();
				registry->AddComponent<TagComponent>(entity);
				registry->AddComponent<TransformComponent>(entity);
				registry->AddComponent<SpotLightComponent>(entity);
				registry->GetComponent<TagComponent>(entity).name = "Spot Light";
				registry->SetActiveEntity(entity);
			}

			ImGui::EndMenu();
		}

		if (ImGui::MenuItem(TITLE_EP(std::string(ICON_FA_WATER) + " Water")))
		{
			auto entity = registry->CreateEntity();
			registry->AddComponent<TagComponent>(entity);
			registry->AddComponent<TransformComponent>(entity);
			registry->AddComponent<WaterComponent>(entity);

			registry->GetComponent<TagComponent>(entity).name = "Water";
			registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(25, 0.001, 25);
			registry->SetActiveEntity(entity);
		}

        ImGui::EndPopup();
    }
}