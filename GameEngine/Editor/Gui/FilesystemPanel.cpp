#include "FilesystemPanel.h"

bool FilesystemPanel::EnablePreview = false;

void FilesystemPanel::Update()
{

}

void FilesystemPanel::Render()
{
	if (GlobalSettings::GameViewActive)
		return;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	if (ImGui::Begin(TITLE_FP("Filesystem")))
	{
		auto size = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);

		if (ImGui::BeginChild(TITLE_FP("Folders"), ImVec2(size.x / 5, size.y)))
		{
			RenderFolderSystem();
			ImGui::EndChild();
		}

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.175f, 0.175f, 0.175f, 1.f));

		if (ImGui::BeginChild(TITLE_FP("Files"), ImVec2(ImGui::GetContentRegionAvail().x, size.y)))
		{
			RenderFileSystem();
			ImGui::EndChild();
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
	}

	ImGui::End();
	ImGui::PopStyleVar();
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
	static auto noIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/NoTexture.png");
	static auto folderIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/FolderIcon.png");
	static auto fileIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/FileIcon.png");
	static auto leftArrowIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/LeftArrow.png");
	static auto rightArrowIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/RightArrow.png");
	static auto pngIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/png.png");
	static auto jpgIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/jpg.png");
	static auto wavIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/wav.png");
	static auto mp3Icon = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/mp3.png");
	static auto objIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/obj.png");
	static auto fbxIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/fbx.png");
	static auto daeIcon = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/dae.png");
	static std::string dropPath;
	static auto currentPath = std::filesystem::absolute(std::filesystem::path("../Assets"));

	auto directory_iterator = std::filesystem::directory_iterator(currentPath);
	std::deque<std::filesystem::directory_entry> queue;

	static std::string selectedPath = "";

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

	//Left Arrow navigation button
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 5));
	if (ImGui::ImageButton(TITLE_FP("Left Arrow"), (ImTextureID)leftArrowIcon->GetTextureID(), ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0)))
	{
		currentPath = currentPath.parent_path();
		selectedPath = "";
	}

	//Right Arrow navigation button
	ImGui::SameLine();
	if (ImGui::ImageButton(TITLE_FP("Right Arrow"), (ImTextureID)rightArrowIcon->GetTextureID(), ImVec2(16, 16), ImVec2(0, 1), ImVec2(1, 0)))
	{
		if(queue.front().is_directory())
			currentPath = queue.front().path();
		selectedPath = "";
	}

	//Path string button
	ImGui::SameLine();
	int pathButtonWidth = glm::max<int>(150, ImGui::GetContentRegionAvail().x - 150);
	ImGui::Button(currentPath.string().c_str(), ImVec2(pathButtonWidth, 20));

	//Enable preview
	ImGui::SameLine();
	int previewButtonWidth = glm::max<int>(150, ImGui::GetContentRegionAvail().x);
	
	if (EnablePreview)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.85, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0.65, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0.45, 0, 1));
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));
	}

	if (ImGui::Button("Enable Preview", ImVec2(previewButtonWidth, 20)))
	{
		EnablePreview = !EnablePreview;
	}

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();

	static int cardWidth = 110;
	static int cardHeight = 150;

	float width = ImGui::GetContentRegionAvail().x;
	int columns = glm::max<int>(1, width / (cardWidth + 5));
	int counter = 0;

	for (auto entry : queue)
	{
		bool is_focused = false;
		std::string path = std::filesystem::path(entry).string();
		std::string name = std::filesystem::path(entry).filename().string();

		std::replace(path.begin(), path.end(), '\\', '/');

		//Clicked on folder
		if (selectedPath != "" && selectedPath == path && entry.is_directory())
		{
			currentPath = path;
			selectedPath = "";
		}

		if (selectedPath == path)
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.66f, 0.66f, 0.12f, 1));
		else
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 2);
		if (ImGui::BeginChild(name.c_str(), ImVec2(cardWidth, cardHeight), true))
		{
			ImVec2 startPos = ImGui::GetCursorScreenPos();		

			is_focused = ImGui::IsWindowFocused();

			auto texture = noIcon;

			if (entry.is_directory())
			{
				texture = folderIcon;
			}
			else if (entry.is_regular_file())
			{
				std::string extension = std::filesystem::path(entry).extension().string();
				texture = fileIcon;

				bool isSound = extension == ".mp3" || extension == ".wav";
				bool isImage = extension == ".png" || extension == ".jpg";
				bool isModel = extension == ".obj" || extension == ".dae" || extension == ".fbx";
				bool isAnimation = extension == ".obj" || extension == ".dae" || extension == ".fbx";

				if (extension == ".mp3")
					texture = mp3Icon;
				else if (extension == ".wav")
					texture = wavIcon;
				else
				{
					if (EnablePreview)
					{
						if (isImage)
							texture = TextureManager::Instance()->LoadImageTexture(path);
						else if (isModel)
						{
							auto model = ModelManager::Instance()->LoadModel(path);
							
							if (model->hasAnimation)
							{
								ModelManager::Instance()->LoadAnimation(path);
								PreviewRenderer::activeAnimationSet.insert(path);
							}

							if (PreviewManager::Instance()->HasAnimationPreview(path))
							{
								texture = PreviewManager::Instance()->GetAnimationPreview(path);
							}
							else if (PreviewManager::Instance()->HasModelPreview(path))
							{
								texture = PreviewManager::Instance()->GetModelPreview(path);
							}
						}
					}
					else
					{
						if (extension == ".png")
							texture = pngIcon;
						else if (extension == ".jpg")
							texture = jpgIcon;
						else if (extension == ".obj")
							texture = objIcon;
						else if (extension == ".fbx")
							texture = fbxIcon;
						else if (extension == ".dae")
							texture = daeIcon;
					}
				}

				ImGui::InvisibleButton(("##" + path).c_str(), ImVec2(cardWidth, cardHeight), ImGuiButtonFlags_AllowOverlap);
				ImGui::SetCursorScreenPos(startPos); // Reset cursor position to the start of the child

				if (selectedPath == path && ImGui::BeginDragDropSource())
				{
					auto p = selectedPath;
					auto s = selectedPath.size();
					std::string payloadName = "None";

					if (isSound)
					{
						payloadName = "Sound";
					}
					else if (isImage)
					{
						payloadName = "Image";
					}
					else if (isModel)
					{
						payloadName = "Model";
					}


					ImGui::SetDragDropPayload(payloadName.c_str(), selectedPath.c_str(), selectedPath.size());
					ImGui::Image((ImTextureID)texture->GetTextureID(), ImVec2(cardWidth, cardWidth), ImVec2(0, 1), ImVec2(1, 0));
					ImGui::Text(name.c_str());
					ImGui::EndDragDropSource();
				}
			}

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
			ImGui::Image((ImTextureID)texture->GetTextureID(), ImVec2(cardWidth, cardWidth), ImVec2(0, 1), ImVec2(1, 0));
			ImGui::PopStyleVar();

			ImGui::Separator();
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(name.c_str()).x) * 0.5f);
			ImGui::SetCursorPosY(cardWidth + (40 - ImGui::CalcTextSize(name.c_str()).y) * 0.5f);
			ImGui::Text(name.c_str());
		}

		ImGui::EndChild();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();

		if ((counter + 1) % columns != 0)
			ImGui::SameLine();

		if (is_focused)
		{
			selectedPath = path;
			std::replace(selectedPath.begin(), selectedPath.end(), '\\', '/');
		}

		counter++;
	}
}