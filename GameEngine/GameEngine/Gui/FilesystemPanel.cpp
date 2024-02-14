#include "FilesystemPanel.h"

void FilesystemPanel::Update()
{

}

void FilesystemPanel::Render()
{
	if (ImGui::Begin(TITLE_FP("Filesystem")))
	{
		auto size = ImGui::GetContentRegionAvail();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.125, 0.125, 0.125, 1));
		if (ImGui::BeginChild(TITLE_FP("Folders"), ImVec2(size.x / 4, size.y)))
		{

			ImGui::EndChild();
		}
		ImGui::PopStyleColor();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.175, 0.175, 0.175, 1));
		if (ImGui::BeginChild(TITLE_FP("Files"), ImVec2(ImGui::GetContentRegionAvail().x, size.y)))
		{
			ImGui::EndChild();
		}
		ImGui::PopStyleColor();
	}

	ImGui::End();
}