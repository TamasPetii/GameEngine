#include "FilesystemPanel.h"

void FilesystemPanel::Update()
{

}

void FilesystemPanel::Render()
{
	if (GlobalSettings::GameViewActive)
		return;

	if (ImGui::Begin(TITLE_FP("Filesystem")))
	{
		auto size = ImGui::GetContentRegionAvail();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.175, 0.175, 0.175, 1));
		if (ImGui::BeginChild(TITLE_FP("Folders"), ImVec2(size.x / 5, size.y)))
		{
			RenderFolderSystem();
			ImGui::EndChild();
		}
		ImGui::PopStyleColor();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.175, 0.175, 0.175, 1));
		if (ImGui::BeginChild(TITLE_FP("Files"), ImVec2(ImGui::GetContentRegionAvail().x, size.y)))
		{
			RenderFileSystem();
			ImGui::EndChild();
		}
		ImGui::PopStyleColor();

	}

	ImGui::End();
}

void FilesystemPanel::RenderFolderSystem(const std::string& folderPath)
{
	auto path = std::filesystem::path(folderPath);
	auto directory_iterator = std::filesystem::directory_iterator(path);

	for (auto& entry : directory_iterator)
	{
		if (entry.is_directory())
		{
			bool open = ImGui::TreeNode(TITLE_FP(std::string(ICON_FA_FOLDER) + " " + entry.path().filename().string() + "##FolderSystem"));

			if (open)
			{
				RenderFolderSystem(entry.path().string());
				ImGui::TreePop();
			}
		}
	}
}
void FilesystemPanel::RenderFileSystem()
{
	static auto folderIcon = TextureManager::Instance()->LoadImageTexture("../Assets/FoderIcon.png");
	static auto fileIcon = TextureManager::Instance()->LoadImageTexture("../Assets/FileIcon.png");
	static auto parentIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Parent.png");
	static auto leftArrowIcon = TextureManager::Instance()->LoadImageTexture("../Assets/LeftArrow.png");
	static auto rightArrowIcon = TextureManager::Instance()->LoadImageTexture("../Assets/RightArrow.png");
	static auto pngIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Gui/png.png");
	static auto jpgIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Gui/jpg.png");
	static auto wavIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Gui/wav.png");
	static auto mp3Icon = TextureManager::Instance()->LoadImageTexture("../Assets/Gui/mp3.png");
	static auto objIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Gui/obj.png");
	static auto fbxIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Gui/fbx.png");
	static std::string dropPath;
	static auto path = std::filesystem::absolute(std::filesystem::path("../Assets"));

	auto directory_iterator = std::filesystem::directory_iterator(path);
	std::deque<std::filesystem::directory_entry> queue;

	for (auto entry : directory_iterator)
	{
		if (entry.is_directory())
		{
			queue.push_front(entry);
		}
		else if (entry.is_regular_file())
		{
			queue.push_back(entry);
		}
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 5));
	if (ImGui::ImageButton(TITLE_FP("Left Arrow"), (ImTextureID)leftArrowIcon->GetTextureID(), ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0)))
	{
		path = path.parent_path();
	}
	ImGui::SameLine();
	if (ImGui::ImageButton(TITLE_FP("Right Arrow"), (ImTextureID)rightArrowIcon->GetTextureID(), ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0)))
	{
		if(queue.front().is_directory())
			path = queue.front().path();
	}
	ImGui::SameLine();
	ImGui::Button(path.string().c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 20));
	ImGui::PopStyleVar();

	int columns = glm::max(ImGui::GetContentRegionAvail().x / 80.f, 1.f);
	int count = 0;

	if (ImGui::BeginTable(TITLE_FP("FileSystemTable"), columns, ImGuiTableColumnFlags_NoResize))
	{
		ImGui::TableNextColumn();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton(TITLE_FP("Parent"), (ImTextureID)parentIcon->GetTextureID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0)))
		{
			path = path.parent_path();
		}

		count++;

		for (auto entry : queue)
		{
			if (count == columns)
			{
				count = 0;
				ImGui::TableNextRow();
			}

			ImGui::TableNextColumn();

			if (entry.is_directory())
			{
				if (ImGui::ImageButton(TITLE_FP(entry.path().string()), (ImTextureID)folderIcon->GetTextureID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0)))
				{
					path = entry.path();
				}

				ImGui::Text(entry.path().filename().string().c_str());
			}
			else if (entry.is_regular_file())
			{
				auto texture = fileIcon;
				std::string fileExt = std::filesystem::path(entry).extension().string();

				if (fileExt == ".mp3")
					texture = mp3Icon;
				else if (fileExt == ".wav")
					texture = wavIcon;
				else if (fileExt == ".png")
					texture = pngIcon;
				else if (fileExt == ".jpg")
					texture = jpgIcon;
				else if (fileExt == ".obj")
					texture = objIcon;
				else if (fileExt == ".fbx")
					texture = fbxIcon;

				ImGui::ImageButton(TITLE_FP(entry.path().string()), (ImTextureID)texture->GetTextureID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
				
				if (fileExt == ".png" || fileExt == ".jpg")
				{
					if (ImGui::BeginDragDropSource())
					{
						dropPath = entry.path().string();
						ImGui::SetDragDropPayload("FileSystem_Image", &dropPath, sizeof(std::string));
						ImGui::Image((ImTextureID)texture->GetTextureID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
						ImGui::Text(dropPath.c_str());
						ImGui::EndDragDropSource();
					}
				}
						
				ImGui::Text(entry.path().filename().string().c_str());
			}

			count++;
		}
		ImGui::PopStyleColor(1);

		ImGui::EndTable();
	}
}