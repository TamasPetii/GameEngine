#include "EntitiesPanel.h"

void EntitiesPanel::Update()
{

}

void EntitiesPanel::Render()
{
	if (ImGui::Begin(TITLE_EP("Entities")))
	{
		auto& registry = Renderer::Instance()->m_Registry;
		for (const auto& entity : registry->GetActiveEntities())
			DisplayEntity(registry, entity);
	}

	ImGui::End();
}

void EntitiesPanel::DisplayEntity(std::shared_ptr<Registry> registry, Entity entity)
{
	if(registry->HasComponent<ShapeComponent>(entity) && registry->GetComponent<ShapeComponent>(entity).isInstanced)
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
	else
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));

	bool open = ImGui::TreeNodeEx(TITLE_EP("Entity - " + std::to_string(entity)));
	ImGui::PopStyleColor();

	if (ImGui::IsItemClicked())
	{
		registry->SetActiveEntity(entity);
	}

	if (open)
	{
		ImGui::TreePop();
	}
}