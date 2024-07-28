#include "ComponentPanel.h"

void ComponentPanel::Update(std::shared_ptr<Scene> scene)
{

}

void ComponentPanel::Render(std::shared_ptr<Scene> scene)
{
	if (ImGui::Begin(TITLE_CP("Components")))
	{
        auto& registry = scene->GetRegistry();
		auto activeEntity = registry->GetActiveEntity();

        if (registry->HasComponent<TagComponent>(activeEntity))
            RenderTagComponent(registry, activeEntity);

		if (registry->HasComponent<TransformComponent>(activeEntity))
			RenderTransformComponent(registry, activeEntity);

        if (registry->HasComponent<ShapeComponent>(activeEntity))
            RenderShapeComponent(registry, activeEntity);

        if (registry->HasComponent<MaterialComponent>(activeEntity))
            RenderMaterialComponent(registry, activeEntity);

        if (registry->HasComponent<DirlightComponent>(activeEntity))
            RenderDirlightComponent(registry, activeEntity);
        
        if (registry->HasComponent<PointLightComponent>(activeEntity))
            RenderPointLightComponent(registry, activeEntity);

        if (registry->HasComponent<SpotLightComponent>(activeEntity))
            RenderSpotLightComponent(registry, activeEntity);

        if (registry->HasComponent<WaterComponent>(activeEntity))
            RenderWaterComponent(registry, activeEntity);

        RenderAddComponentPopUp(registry, activeEntity);
	}

	ImGui::End();
}

void ComponentPanel::RenderTagComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<TagComponent>(entity);
    static bool visible = true;

    if (ImGui::CollapsingHeader(TITLE_CP(std::string(ICON_FA_TAGS) + " TagComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 4.f;
        std::vector<char> name(100);
        std::vector<char> tag(100);
        std::copy(component.name.begin(), component.name.end(), name.begin());
        std::copy(component.tag.begin(), component.tag.end(), tag.begin());

        ImGui::Text("Name");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::InputText("Name", name.data(), name.size()))
        {
            component.name = std::string(name.data());
        }

        ImGui::Text("Tag");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::InputText("Tag", tag.data(), tag.size()))
        {
            component.tag = std::string(tag.data());
        }
    }

    if (visible == false)
        registry->RemoveComponent<TagComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderTransformComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<TransformComponent>(entity);

    static bool visible = true;
	if (ImGui::CollapsingHeader(TITLE_CP(std::string(ICON_FA_EXPAND_ARROWS_ALT) + " TransformComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
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

    if (visible == false)
        registry->RemoveComponent<TransformComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderMaterialComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<MaterialComponent>(entity);

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP(std::string(ICON_FA_PALETTE) + " MaterialComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
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

        ImGui::Text("Scale");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat2(TITLE_CP("##TextureScale##MaterialComponent"), &component.textureScale.x, 0.1f))
        {
            registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Use Bloom");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Checkbox(TITLE_CP("##UseBloom##MaterialComponent"), &component.useBloom))
        {
            registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Textures");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);

        static auto noDiffuseTexture = TextureManager::Instance()->LoadImageTexture("../Assets/NoDiffuseTexture.png");
        static auto noSpecularTexture = TextureManager::Instance()->LoadImageTexture("../Assets/NoSpecularTexture.png");
        static auto noNormalTexture = TextureManager::Instance()->LoadImageTexture("../Assets/NoNormalTexture.png");
        if (ImGui::BeginTable(TITLE_CP("TextureTable"), 2, ImGuiTableFlags_Borders))
        {
            {
                ImGui::TableNextRow();

                //Diffuse Texture
                {
                    static bool isButtonPressed = false;
                    GLuint textureID = component.diffuse != nullptr ? component.diffuse->GetTextureID() : noDiffuseTexture->GetTextureID();
                    ImGui::TableSetColumnIndex(0);
                    if (ImGui::ImageButton((ImTextureID)textureID, ImVec2(128, 128)))
                    {
                        isButtonPressed = true;
                    }

                    /*
                    if (ImGui::BeginDragDropTarget())
                    {
                        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FileSystem_Image"));
                        {
                            std::string message = std::string((const char*)payload->Data);
                            std::cout << "Got Message = " << message << std::endl;
                        }
                        ImGui::EndDragDropTarget();
                    }
                    */

                    if (ImGui::IsItemHovered() && component.diffuse != nullptr)
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text("Specular Texture");
                        ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
                        ImGui::EndTooltip();
                    }

                    ImGui::Text("Diffuse");

                    if (isButtonPressed)
                    {
                        ImGui::OpenPopup("Texture Assets");
                        auto message = TextureAssetPopup();
                        if (message.first)
                        {
                            isButtonPressed = false;
                            if (message.second != nullptr)
                            {
                                component.diffuse = message.second;
                                registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
                            }
                        }
                    }
                }

                //Specular Texture
                {
                    static bool isButtonPressed = false;
                    GLuint textureID = component.specular != nullptr ? component.specular->GetTextureID() : noSpecularTexture->GetTextureID();
                    ImGui::TableSetColumnIndex(1);
                    
                    if (ImGui::ImageButton((ImTextureID)textureID, ImVec2(128, 128)))
                    {
                        isButtonPressed = true;
                    }

                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text("Specular Texture");
                        ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
                        ImGui::EndTooltip();
                    }

                    ImGui::Text("Specular");

                    if (isButtonPressed)
                    {
                        ImGui::OpenPopup("Texture Assets");
                        auto message = TextureAssetPopup();
                        if (message.first)
                        {
                            isButtonPressed = false;
                            if (message.second != nullptr)
                            {
                                component.specular = message.second;
                                registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
                            }
                        }
                    }
                }

                ImGui::TableNextRow();

                //Normal Texture
                {
                    static bool isButtonPressed = false;
                    GLuint textureID = component.normal != nullptr ? component.normal->GetTextureID() : noNormalTexture->GetTextureID();
                    ImGui::TableSetColumnIndex(0);
                    if (ImGui::ImageButton((ImTextureID)textureID, ImVec2(128, 128)))
                    {
                        isButtonPressed = true;
                    }

                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text("Normal Texture");
                        ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
                        ImGui::EndTooltip();
                    }

                    ImGui::Text("Normal");

                    if (isButtonPressed)
                    {
                        ImGui::OpenPopup("Texture Assets");
                        auto message = TextureAssetPopup();
                        if (message.first)
                        {
                            isButtonPressed = false;
                            if (message.second != nullptr)
                            {
                                component.normal = message.second;
                                registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
                            }
                        }
                    }
                }
            }

            ImGui::EndTable();
        }
    }

    if (visible == false)
        registry->RemoveComponent<MaterialComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderDirlightComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<DirlightComponent>(entity);

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("DirlightComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
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

    if (visible == false)
        registry->RemoveComponent<DirlightComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderPointLightComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<PointLightComponent>(entity);

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("PointLightComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
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

    if (visible == false)
        registry->RemoveComponent<PointLightComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderSpotLightComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<SpotLightComponent>(entity);

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("SpotLightComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
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

        ImGui::Text("Shadow Texture");
        ImGui::SameLine();
        ImGui::Image((ImTextureID)component.shadowTexture, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
    }

    if (visible == false)
        registry->RemoveComponent<SpotLightComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderShapeComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<ShapeComponent>(entity);

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("ShapeComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 3.f;

        ImGui::Text("Cast Shadow");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Checkbox(TITLE_CP("##CastShadow##ShapeComponent"), &component.castShadow))
        {
            registry->SetFlag<ShapeComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Receive Shadow");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Checkbox(TITLE_CP("##ReceiveShadow##ShapeComponent"), &component.receiveShadow))
        {
            registry->SetFlag<ShapeComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Is Instanced");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Checkbox(TITLE_CP("##IsInstanced##ShapeComponent"), &component.isInstanced))
        {
            registry->SetFlag<ShapeComponent>(entity, UPDATE_FLAG);
        }
        
        ImGui::Text("Shape");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::ImageButton(TITLE_CP("##Shape##ShapeComponent"), 0, ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0)))
        {
            registry->SetFlag<ShapeComponent>(entity, UPDATE_FLAG);
        }
    }

    if (visible == false)
        registry->RemoveComponent<ShapeComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderWaterComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<WaterComponent>(entity);

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("WaterComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 3.f;

        ImGui::Text("Water TexSize");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragInt2(TITLE_CP("##WaterTexSize##WaterComponent"), &component.size.x, 1.f, 1, 4096))
        {
            registry->SetFlag<WaterComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Dudv Scale");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat2(TITLE_CP("##DudvScale##WaterComponent"), &component.dudvScale.x))
        {
            registry->SetFlag<WaterComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Wave Strength");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat2(TITLE_CP("##WaveStrength##ShapeComponent"), &component.dudvWaveStrength.x, 0.005f, 0.f, 2.f))
        {
            registry->SetFlag<WaterComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Wave Speed");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat(TITLE_CP("##Wave Speed##ShapeComponent"), &component.dudvMoveSpeed, 0.005f, 0.f, 1.f))
        {
            registry->SetFlag<WaterComponent>(entity, UPDATE_FLAG);
        }

        //DuDv Texture
        {
            static auto noTexture = TextureManager::Instance()->LoadImageTexture("../Assets/NoNormalTexture.png");

            static bool isButtonPressed = false;
            GLuint textureID = component.dudv != nullptr ? component.dudv->GetTextureID() : noTexture->GetTextureID();

            ImGui::Text("DuDv Texture");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
            float width = glm::min(ImGui::GetContentRegionAvail().x - 10, 512.f);
            if (ImGui::ImageButton((ImTextureID)textureID, ImVec2(width, width)))
            {
                isButtonPressed = true;
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("DuDv Texture");
                ImGui::Image((ImTextureID)textureID, ImVec2(512, 512));
                ImGui::EndTooltip();
            }

            if (isButtonPressed)
            {
                ImGui::OpenPopup("Texture Assets");
                auto message = TextureAssetPopup();
                if (message.first)
                {
                    isButtonPressed = false;
                    if (message.second != nullptr)
                    {
                        component.dudv = message.second;
                        registry->SetFlag<WaterComponent>(entity, UPDATE_FLAG);
                    }
                }
            }
        }
    }

    if (visible == false)
        registry->RemoveComponent<WaterComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderAddComponentPopUp(std::shared_ptr<Registry> registry, Entity entity)
{
    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::MenuItem(TITLE_CP("Transform Component"), NULL, registry->HasComponent<TransformComponent>(entity)))
        {
            if (!registry->HasComponent<TransformComponent>(entity))
                registry->AddComponent<TransformComponent>(entity);
        }

        if (ImGui::MenuItem(TITLE_CP("Material Component"), NULL, registry->HasComponent<MaterialComponent>(entity)))
        {
            if (!registry->HasComponent<MaterialComponent>(entity))
                registry->AddComponent<MaterialComponent>(entity);
        }

        if (ImGui::MenuItem(TITLE_CP("Shape Component"), NULL, registry->HasComponent<ShapeComponent>(entity)))
        {
            if (!registry->HasComponent<ShapeComponent>(entity))
                registry->AddComponent<ShapeComponent>(entity);
        }

        if (ImGui::MenuItem(TITLE_CP("Model Component"), NULL, registry->HasComponent<ModelComponent>(entity)))
        {
            if (!registry->HasComponent<ModelComponent>(entity))
                registry->AddComponent<ModelComponent>(entity);
        }

        if (ImGui::MenuItem(TITLE_CP("Dirlight Component"), NULL, registry->HasComponent<DirlightComponent>(entity)))
        {
            if (!registry->HasComponent<DirlightComponent>(entity))
                registry->AddComponent<DirlightComponent>(entity);
        }

        if (ImGui::MenuItem(TITLE_CP("Pointlight Component"), NULL, registry->HasComponent<PointLightComponent>(entity)))
        {
            if (!registry->HasComponent<PointLightComponent>(entity))
                registry->AddComponent<PointLightComponent>(entity);
        }

        if (ImGui::MenuItem(TITLE_CP("Spotlight Component"), NULL, registry->HasComponent<SpotLightComponent>(entity)))
        {
            if (!registry->HasComponent<SpotLightComponent>(entity))
                registry->AddComponent<SpotLightComponent>(entity);
        }

        if (ImGui::MenuItem(TITLE_CP("Water Component"), NULL, registry->HasComponent<WaterComponent>(entity)))
        {
            if (!registry->HasComponent<WaterComponent>(entity))
                registry->AddComponent<WaterComponent>(entity);
        }

        ImGui::EndPopup();
    }
}

std::pair<bool, std::shared_ptr<TextureGL>> ComponentPanel::TextureAssetPopup()
{
    static std::shared_ptr<TextureGL> selected = nullptr;
    static bool isSelected = false;
    if (ImGui::BeginPopupModal("Texture Assets", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (ImGui::BeginTable("Texture Assets Table", 5))
        {
            int index = 0;
            for (const auto& texture : TextureManager::Instance()->GetAllTexture())
            {
                if (index % 5 == 0)
                {
                    ImGui::TableNextRow();
                    index = 0;
                }

                ImGui::TableSetColumnIndex(index++);

                if (texture.second == selected)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 0.f, 0.f, 1.f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.f, 0.f, 0.f, 1.f));
                }

                if (ImGui::ImageButton((ImTextureID)texture.second->GetTextureID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0), 3))
                {
                    selected = texture.second;
                }

                if (texture.second == selected)
                    ImGui::PopStyleColor(2);

                if (ImGui::IsItemHovered())
                {
                    ImGui::BeginTooltip();
                    ImGui::Text("File Name : %s", std::filesystem::path(texture.first).filename().string().c_str());
                    ImGui::Text("File Path : %s", std::filesystem::path(texture.first).relative_path().string().c_str());
                    ImGui::Image((ImTextureID)texture.second->GetTextureID(), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
                    ImGui::EndTooltip();
                }
            }

            ImGui::EndTable();
        }

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 1, 0, 1));
        if (ImGui::Button("OK", ImVec2(120, 0))) 
        { 
            ImGui::CloseCurrentPopup();
            isSelected = true;
        }
        ImGui::PopStyleColor();

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 0, 0, 1));
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        { 
            ImGui::CloseCurrentPopup();
            isSelected = true;
        }
        ImGui::PopStyleColor();

        ImGui::EndPopup();
    }

    if (isSelected)
    {
        std::pair<bool, std::shared_ptr<TextureGL>> message = { true, selected };
        selected = nullptr;
        isSelected = false;
        return message;
    }

    return { false, nullptr };
}