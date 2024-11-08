#include "EntitiesPanel.h"

float EntitiesPanel::regionWidth = 0;
std::vector<Entity> EntitiesPanel::toDelete;
std::vector<Entity> EntitiesPanel::toCopy;

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

		auto cursorPos = ImGui::GetCursorPos();
		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x > 0 && size.y > 0)
		{
			ImGui::SetNextItemAllowOverlap();
			ImGui::InvisibleButton("##EntityPanelInvisibleButtonBg", size);

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity_Tree"))
				{
					Entity droppedEntity = *(Entity*)payload->Data;

					if (droppedEntity != null)
					{
						registry->SetParent(droppedEntity, null);
					}
				}

				ImGui::EndDragDropTarget();
			}
		}

		ImGui::SetCursorPos(cursorPos);

		RenderAddEntityPopUp(registry);

		for (const auto& entity : registry->GetActiveEntities())
		{
			regionWidth = ImGui::GetContentRegionAvail().x;

			if(!registry->HasParent(entity))
				DisplayEntity(registry, entity);
		}

		//Delete Entity
		for (auto entity : toDelete)
			registry->DestroyEntity(entity);

		toDelete.clear();

		//Copy Entity
		for (auto entity : toCopy)
		{
			auto newEntity = registry->CreateEntity();

			auto data = registry->SerializeEntity(entity);
			data["entity"] = newEntity;
			data["parent"] = null;
			data["children"] = nlohmann::json::array();
			registry->DeSerializeEntity(data);
			registry->SetActiveEntity(newEntity);
		}

		toCopy.clear();
	}

	ImGui::End();
}

void EntitiesPanel::DisplayEntity(std::shared_ptr<Registry> registry, Entity entity)
{
	std::string name = "Entity";
	if (registry->HasComponent<TagComponent>(entity))
		name = registry->GetComponent<TagComponent>(entity).name;

	if(registry->GetActiveEntity() == entity)
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));
	bool open = ImGui::TreeNodeEx(TITLE_EP(std::string(ICON_FA_CUBE) + " " + name + "##" + std::to_string(entity)), ImGuiTreeNodeFlags_OpenOnDoubleClick);
	
	if (registry->GetActiveEntity() == entity)
		ImGui::PopStyleColor();

	if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
	{
		registry->SetActiveEntity(entity);
	}

	if (ImGui::BeginPopupContextItem(("EntityPopup##" + name + std::to_string(entity)).c_str(), ImGuiMouseButton_Right))
	{
		if (ImGui::Button(("Delete Entity##" + name + std::to_string(entity)).c_str(), ImVec2(120, 20)))
		{
			toDelete.push_back(entity);
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button(("Copy Entity##" + name + std::to_string(entity)).c_str(), ImVec2(120, 20)))
		{
			toCopy.push_back(entity);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();  // End the popup
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
	else if(registry->HasComponent<PointLightComponent>(entity) && !registry->GetComponent<PointLightComponent>(entity).toRender)
		eye = ICON_FA_EYE_SLASH;
	else if (registry->HasComponent<SpotLightComponent>(entity) && !registry->GetComponent<SpotLightComponent>(entity).toRender)
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