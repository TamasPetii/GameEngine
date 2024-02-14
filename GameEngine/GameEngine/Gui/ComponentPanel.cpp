#include "ComponentPanel.h"

void ComponentPanel::Update()
{

}

void ComponentPanel::Render()
{
	if (ImGui::Begin(TITLE_CP("Components")))
	{
		auto registry = Renderer::Instance()->m_Registry;
		auto activeEntity = registry->GetActiveEntity();

		if (registry->HasComponent<TransformComponent>(activeEntity))
			RenderTransformComponent(registry, activeEntity);

        if (registry->HasComponent<MaterialComponent>(activeEntity))
            RenderMaterialComponent(registry, activeEntity);

        if (registry->HasComponent<DirlightComponent>(activeEntity))
            RenderDirlightComponent(registry, activeEntity);
        
        if (registry->HasComponent<PointLightComponent>(activeEntity))
            RenderPointLightComponent(registry, activeEntity);

        if (registry->HasComponent<SpotLightComponent>(activeEntity))
            RenderSpotLightComponent(registry, activeEntity);
	}

	ImGui::End();
}

void ComponentPanel::RenderTransformComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<TransformComponent>(entity);

	if (ImGui::CollapsingHeader(TITLE_CP("TransformComponent"), ImGuiTreeNodeFlags_DefaultOpen))
	{
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 2, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2{ 1, 3 });
		if (ImGui::BeginTable(TITLE_CP("TransformTable"), 4, ImGuiTableFlags_NoBordersInBody))
		{
            //Translation Part
            {
                ImGui::TableNextRow();

                //Translation Text
                {
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Translate");
                }

                //Translation X
                {
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.f, 0.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));

                    if (ImGui::Button("X##TranslateX"))
                    {
                        component.translate.x = 0.f;
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }

                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::DragFloat("##TranslateX", &component.translate.x, 0.075f))
                    {
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }
                }

                //Translation Y
                {
                    ImGui::TableSetColumnIndex(2);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.7f, 0.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.5f, 0.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.3f, 0.f, 1.f));

                    if (ImGui::Button("Y##TranslateY"))
                    {
                        component.translate.y = 0.f;
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }

                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::DragFloat("##TranslateY", &component.translate.y, 0.075f))
                    {
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }
                }

                //Translation Z
                {
                    ImGui::TableSetColumnIndex(3);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 1.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.8f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.5f, 1.f));

                    if (ImGui::Button("Z##TranslateZ"))
                    {
                        component.translate.z = 0.f;
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }

                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::DragFloat("##TranslateZ", &component.translate.z, 0.075f))
                    {
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }
                }
            }

            //Rotation Part
            {
                ImGui::TableNextRow();

                //Rotation Text
                {
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Rotation");
                }

                //Rotation X
                {
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.f, 0.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));

                    if (ImGui::Button("X##RotationX"))
                    {
                        component.rotate.x = 0.f;
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }

                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::DragFloat("##RotationX", &component.rotate.x, 0.075f))
                    {
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }
                }

                //Rotation Y
                {
                    ImGui::TableSetColumnIndex(2);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.7f, 0.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.5f, 0.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.3f, 0.f, 1.f));

                    if (ImGui::Button("Y##RotationXY"))
                    {
                        component.rotate.y = 0.f;
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }

                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::DragFloat("##RotationXY", &component.rotate.y, 0.075f))
                    {
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }
                }

                //Rotation Z
                {
                    ImGui::TableSetColumnIndex(3);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 1.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.8f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.5f, 1.f));

                    if (ImGui::Button("Z##RotationXZ"))
                    {
                        component.rotate.z = 0.f;
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }

                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::DragFloat("##RotationXZ", &component.rotate.z, 0.075f))
                    {
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }
                }
            }

            //Scale Part
            {
                ImGui::TableNextRow();

                //Scale Text
                {
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Scale");
                }

                //Scale X
                {
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.f, 0.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.f, 0.f, 1.f));

                    if (ImGui::Button("X##ScaleX"))
                    {
                        component.scale.x = 1.f;
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }

                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::DragFloat("##ScaleX", &component.scale.x, 0.075f))
                    {
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }
                }

                //Translation Y
                {
                    ImGui::TableSetColumnIndex(2);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.7f, 0.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.5f, 0.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.3f, 0.f, 1.f));

                    if (ImGui::Button("Y##ScaleY"))
                    {
                        component.scale.y = 1.f;
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }

                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::DragFloat("##ScaleY", &component.scale.y, 0.075f))
                    {
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }
                }

                //Translation Z
                {
                    ImGui::TableSetColumnIndex(3);
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 1.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.8f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.5f, 1.f));

                    if (ImGui::Button("Z##ScaleZ"))
                    {
                        component.scale.z = 1.f;
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }

                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::DragFloat("##ScaleZ", &component.scale.z, 0.05f))
                    {
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                    }
                }
            }
		}
        ImGui::PopStyleVar(2);
		ImGui::EndTable();
	}
}

void ComponentPanel::RenderMaterialComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<MaterialComponent>(entity);

    if (ImGui::CollapsingHeader(TITLE_CP("MaterialComponent"), ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 4.f;

        ImGui::Text("Color");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(glm::min<float>(200, ImGui::GetContentRegionAvail().x));
        if (ImGui::ColorPicker4(TITLE_CP("##Color##MaterialComponent"), &component.color.x))
        {
            registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Textures");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);

        if (ImGui::BeginTable(TITLE_CP("TextureTable"), 2, ImGuiTableFlags_Borders))
        {
            {
                ImGui::TableNextRow();

                //Diffuse Texture
                {
                    GLuint textureID = component.diffuse != nullptr ? component.diffuse->GetTextureID() : 0;
                    ImGui::TableSetColumnIndex(0);
                    ImGui::ImageButton((ImTextureID)textureID, ImVec2(128, 128));

                    if (ImGui::IsItemHovered() && component.diffuse != nullptr)
                    {
                        ImGui::BeginTooltip();
                        ImGui::EndTooltip();
                    }

                    ImGui::Text("Diffuse");
                }

                //Specular Texture
                {
                    GLuint textureID = component.specular != nullptr ? component.specular->GetTextureID() : 0;
                    ImGui::TableSetColumnIndex(1);
                    ImGui::ImageButton((ImTextureID)textureID, ImVec2(128, 128));

                    if (ImGui::IsItemHovered() && component.diffuse != nullptr)
                    {
                        ImGui::BeginTooltip();
                        ImGui::EndTooltip();
                    }

                    ImGui::Text("Specular");
                }

                ImGui::TableNextRow();

                //Normal Texture
                {
                    GLuint textureID = component.normal != nullptr ? component.normal->GetTextureID() : 0;
                    ImGui::TableSetColumnIndex(0);
                    if (ImGui::ImageButton((ImTextureID)textureID, ImVec2(128, 128)))
                    {
                        std::string path = FileDialogWindows::ShowFileDialog();
                        if (path != "")
                        {
                            std::cout << "Image path: " << path << std::endl;
                        }
                    }

                    if (ImGui::IsItemHovered() && component.diffuse != nullptr)
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text("Asd");
                        ImGui::EndTooltip();
                    }

                    ImGui::Text("Normal");
                }
            }

            ImGui::EndTable();
        }

        ImGui::Text("Use Bloom");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Checkbox(TITLE_CP("##UseBloom##MaterialComponent"), &component.useBloom))
        {
            registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
        }

    }
}

void ComponentPanel::RenderDirlightComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<DirlightComponent>(entity);

    if (ImGui::CollapsingHeader(TITLE_CP("DirlightComponent"), ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 4.f;

        ImGui::Text("Color");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(glm::min<float>(200, ImGui::GetContentRegionAvail().x));
        if (ImGui::ColorPicker3(TITLE_CP("##Color##DirlightComponent"), &component.color.x))
        {
            registry->SetFlag<DirlightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Direction");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat3(TITLE_CP("##Direction##DirlightComponent"), &component.direction.x, 0.005f, -1.f, 1.f))
        {
            registry->SetFlag<DirlightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Strength");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat(TITLE_CP("##Strength##DirlightComponent"), &component.strength, 0.005f, 0.f, 100.f))
        {
            registry->SetFlag<DirlightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Use Shadow");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Checkbox(TITLE_CP("##UseShadow##DirlightComponent"), &component.useShadow))
        {
            registry->SetFlag<DirlightComponent>(entity, UPDATE_FLAG);
        }
    }
}

void ComponentPanel::RenderPointLightComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<PointLightComponent>(entity);

    if (ImGui::CollapsingHeader(TITLE_CP("PointLightComponent"), ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 4.f;

        ImGui::Text("Color");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(glm::min<float>(200, ImGui::GetContentRegionAvail().x));
        if (ImGui::ColorPicker3(TITLE_CP("##Color##PointLightComponent"), &component.color.x))
        {
            registry->SetFlag<PointLightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Strength");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat(TITLE_CP("##Strength##PointLightComponent"), &component.strength, 0.005f, 0.f, 100.f))
        {
            registry->SetFlag<PointLightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Use Shadow");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Checkbox(TITLE_CP("##UseShadow##PointLightComponent"), &component.useShadow))
        {
            registry->SetFlag<PointLightComponent>(entity, UPDATE_FLAG);
        }
    }
}

void ComponentPanel::RenderSpotLightComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<SpotLightComponent>(entity);

    if (ImGui::CollapsingHeader(TITLE_CP("SpotLightComponent"), ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 4.f;

        ImGui::Text("Color");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(glm::min<float>(200, ImGui::GetContentRegionAvail().x));
        if (ImGui::ColorPicker3(TITLE_CP("##Color##SpotLightComponent"), &component.color.x))
        {
            registry->SetFlag<SpotLightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Direction");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat3(TITLE_CP("##Direction##SpotLightComponent"), &component.direction.x, 0.005f, -1.f, 1.f))
        {
            registry->SetFlag<SpotLightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Strength");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat(TITLE_CP("##Strength##SpotLightComponent"), &component.strength, 0.005f, 0.f, 100.f))
        {
            registry->SetFlag<SpotLightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Length");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat(TITLE_CP("##Length##SpotLightComponent"), &component.length))
        {
            registry->SetFlag<SpotLightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Inner");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat(TITLE_CP("##Inner##SpotLightComponent"), &component.angles.x))
        {
            registry->SetFlag<SpotLightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Outer");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat(TITLE_CP("##Outer##SpotLightComponent"), &component.angles.y))
        {
            registry->SetFlag<SpotLightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Use Shadow");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Checkbox(TITLE_CP("##UseShadow##SpotLightComponent"), &component.useShadow))
        {
            registry->SetFlag<SpotLightComponent>(entity, UPDATE_FLAG);
        }
    }
}