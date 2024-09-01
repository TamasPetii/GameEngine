#include "ComponentPanel.h"

bool ComponentPanel::OpenModelAssetPopup = false;
bool ComponentPanel::OpenShapeAssetPopup = false;
bool ComponentPanel::OpenTextureAssetPopup = false;
bool ComponentPanel::OpenAnimationAssetPopup = false;
TextureAssetType ComponentPanel::textureAssetType;

void ComponentPanel::Update(std::shared_ptr<Scene> scene)
{

}

void ComponentPanel::Render(std::shared_ptr<Scene> scene)
{
    if (GlobalSettings::GameViewActive)
        return;

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

        if (registry->HasComponent<ModelComponent>(activeEntity))
            RenderModelComponent(registry, activeEntity);

        if (registry->HasComponent<AnimationComponent>(activeEntity))
            RenderAnimationComponent(registry, activeEntity);
        else
            PreviewRenderer::animationPreviewType = AnimationPreviewType::NONE;

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
    static auto noTexture = TextureManager::Instance()->LoadImageTexture("../Assets/NoTexture.png");
    auto& component = registry->GetComponent<MaterialComponent>(entity);

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP(std::string(ICON_FA_PALETTE) + " MaterialComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 4.f;

        ImGui::Text("Use Bloom");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Checkbox(TITLE_CP("##UseBloom##MaterialComponent"), &component.useBloom))
        {
            registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Color");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(glm::min<float>(200, ImGui::GetContentRegionAvail().x));
        if (ImGui::ColorPicker4(TITLE_CP("##Color##MaterialComponent"), &component.color.x))
        {
            registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
        }

        float imgWidth = glm::min(width * 3, 512.f);

        //Diffuse Texture
        ImGui::Text("Diffuse");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));

        GLuint diffuseTextureID = component.diffuse != nullptr ? component.diffuse->GetTextureID() : noTexture->GetTextureID();
        if (ImGui::ImageButton(TITLE_CP("##DiffuseTexture##MaterialComponent"), (ImTextureID)diffuseTextureID, ImVec2(imgWidth, imgWidth), ImVec2(0, 1), ImVec2(1, 0)))
        {
            textureAssetType = TextureAssetType::DIFFUSE;
            OpenTextureAssetPopup = true;
        }

        //Specular Texture
        ImGui::Text("Specular");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));

        GLuint specularTextureID = component.specular != nullptr ? component.specular->GetTextureID() : noTexture->GetTextureID();
        if (ImGui::ImageButton(TITLE_CP("##SpecularTexture##MaterialComponent"), (ImTextureID)specularTextureID, ImVec2(imgWidth, imgWidth), ImVec2(0, 1), ImVec2(1, 0)))
        {
            textureAssetType = TextureAssetType::SPECULAR;
            OpenTextureAssetPopup = true;
        }

        //Normal Texture
        ImGui::Text("Normal");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));

        GLuint normalTextureID = component.normal != nullptr ? component.normal->GetTextureID() : noTexture->GetTextureID();
        if (ImGui::ImageButton(TITLE_CP("##NormalTexture##MaterialComponent"), (ImTextureID)normalTextureID, ImVec2(imgWidth, imgWidth), ImVec2(0, 1), ImVec2(1, 0)))
        {
            textureAssetType = TextureAssetType::NORMAL;
            OpenTextureAssetPopup = true;
        }

        ImGui::Text("TexScale");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat2(TITLE_CP("##TextureScale##MaterialComponent"), &component.textureScale.x, 0.1f))
        {
            registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
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
    auto previewManager = PreviewManager::Instance();
    auto& component = registry->GetComponent<ShapeComponent>(entity);
    static auto noTexture = TextureManager::Instance()->LoadImageTexture("../Assets/NoTexture.png");

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
        float imgWidth = glm::min(ImGui::GetContentRegionAvail().x - 10, 512.f);

        bool hasShape = component.shape != nullptr;
        bool hasShapePreview = hasShape && previewManager->HasShapePreview(component.shape->GetName());
        auto textureID = hasShapePreview ? previewManager->GetShapePreview(component.shape->GetName()) : noTexture;
        if (ImGui::ImageButton(TITLE_CP("##Shape##ShapeComponent"), (ImTextureID)textureID->GetTextureID(), ImVec2(imgWidth, imgWidth), ImVec2(0, 1), ImVec2(1, 0)))
        {
            OpenShapeAssetPopup = true;
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
            static auto noTexture = TextureManager::Instance()->LoadImageTexture("../Assets/NoTexture.png");
            GLuint textureID = component.dudv != nullptr ? component.dudv->GetTextureID() : noTexture->GetTextureID();

            ImGui::Text("DuDv Texture");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
            float width = glm::min(ImGui::GetContentRegionAvail().x - 10, 512.f);
            if (ImGui::ImageButton((ImTextureID)textureID, ImVec2(width, width)))
            {
                textureAssetType = TextureAssetType::DUDV;
                OpenTextureAssetPopup = true;
            }

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("DuDv Texture");
                ImGui::Image((ImTextureID)textureID, ImVec2(512, 512));
                ImGui::EndTooltip();
            }
        }

        if(ImGui::CollapsingHeader("Debug Render Texture"))
        {
            float width = 0;

            ImGui::Text("Reflection");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
            width = glm::min(ImGui::GetContentRegionAvail().x - 10, 512.f);
            ImGui::ImageButton((ImTextureID)component.reflectionFbo->GetTextureID("reflection"), ImVec2(width, width));
        
            ImGui::Text("Refraction");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
            width = glm::min(ImGui::GetContentRegionAvail().x - 10, 512.f);
            ImGui::ImageButton((ImTextureID)component.refractionFbo->GetTextureID("refraction"), ImVec2(width, width));
        }
    }

    if (visible == false)
        registry->RemoveComponent<WaterComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderModelComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto previewManager = PreviewManager::Instance();
    auto& component = registry->GetComponent<ModelComponent>(entity);
    static auto noTexture = TextureManager::Instance()->LoadImageTexture("../Assets/NoTexture.png");

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("ModelComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 3.f;

        ImGui::Text("Cast Shadow");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Checkbox(TITLE_CP("##CastShadow##ModelComponent"), &component.castShadow))
        {
            registry->SetFlag<ModelComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Receive Shadow");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Checkbox(TITLE_CP("##ReceiveShadow##ModelComponent"), &component.receiveShadow))
        {
            registry->SetFlag<ModelComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Is Instanced");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Checkbox(TITLE_CP("##IsInstanced##ModelComponent"), &component.isInstanced))
        {
            registry->SetFlag<ModelComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Model");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        float imgWidth = glm::min(ImGui::GetContentRegionAvail().x - 10, 512.f);

        bool hasModel = component.model != nullptr;
        bool hasModelPreview = hasModel && previewManager->HasModelPreview(component.model->GetPath());
        auto textureID = hasModelPreview ? previewManager->GetModelPreview(component.model->GetPath()) : noTexture;
        if (ImGui::ImageButton(TITLE_CP("##Model##ModelComponent"), (ImTextureID)textureID->GetTextureID(), ImVec2(imgWidth, imgWidth), ImVec2(0, 1), ImVec2(1, 0)))
        {
            OpenModelAssetPopup = true;
        }

        /*
        std::string path = component.model ? component.model->GetPath() : "none";
        ImGui::Text("Model");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Button(TITLE_CP(path + "##Model##ModelComponent"), ImVec2(ImGui::GetContentRegionAvail().x, 16)))
        {
            FileDialogOption option;
            option.dialogType = FileDialogType::OPEN_DIALOG;
            option.returnType = FileDialogReturnType::PICK_FILE;
            option.filters.push_back({ L"Model", L"*.obj;*.dae;*.fbx" });
            std::string path = FileDialogWindows::ShowFileDialog(option);

            if (std::filesystem::exists(path) &&
                std::filesystem::path(path).extension() == ".obj" ||
                std::filesystem::path(path).extension() == ".dae" ||
                std::filesystem::path(path).extension() == ".fbx")
                component.model = ModelManager::Instance()->LoadModel(path);

            registry->SetFlag<ModelComponent>(entity, UPDATE_FLAG);
        }
        */
    }

    if (visible == false)
        registry->RemoveComponent<ModelComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderAnimationComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    if(PreviewRenderer::animationPreviewType == AnimationPreviewType::NONE)
        PreviewRenderer::animationPreviewType = AnimationPreviewType::ACTIVE_ANIMATION;

    auto previewManager = PreviewManager::Instance();
    auto& component = registry->GetComponent<AnimationComponent>(entity);
    static auto noTexture = TextureManager::Instance()->LoadImageTexture("../Assets/NoTexture.png");

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("AnimationComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 3.f;

        ImGui::Text("Animation");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        float imgWidth = glm::min(ImGui::GetContentRegionAvail().x - 10, 512.f);

        bool hasAnimation = component.animation != nullptr;
        bool hasAnimationPreview = hasAnimation && previewManager->HasAnimationPreview(component.animation->GetPath());
        auto textureID = hasAnimationPreview ? previewManager->GetAnimationPreview(component.animation->GetPath()) : noTexture;
        if (ImGui::ImageButton(TITLE_CP("##Animation##AnimationComponent"), (ImTextureID)textureID->GetTextureID(), ImVec2(imgWidth, imgWidth), ImVec2(0, 1), ImVec2(1, 0)))
        {
            OpenAnimationAssetPopup = true;
        }

        /*
        FileDialogOption option;
        option.dialogType = FileDialogType::OPEN_DIALOG;
        option.returnType = FileDialogReturnType::PICK_FILE;
        option.filters.push_back({ L"Animation", L"*.dae;*.fbx" });
        std::string path = FileDialogWindows::ShowFileDialog(option);

        if (std::filesystem::exists(path) &&
            std::filesystem::path(path).extension() == ".dae" ||
            std::filesystem::path(path).extension() == ".fbx")
            component.animation = ModelManager::Instance()->LoadAnimation(path);
        */

        registry->SetFlag<AnimationComponent>(entity, UPDATE_FLAG);
    }

    if (visible == false)
        registry->RemoveComponent<AnimationComponent>(entity);

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

        if (ImGui::MenuItem(TITLE_CP("Animation Component"), NULL, registry->HasComponent<AnimationComponent>(entity)))
        {
            if (!registry->HasComponent<AnimationComponent>(entity))
                registry->AddComponent<AnimationComponent>(entity);
        }

        if (ImGui::MenuItem(TITLE_CP("Water Component"), NULL, registry->HasComponent<WaterComponent>(entity)))
        {
            if (!registry->HasComponent<WaterComponent>(entity))
                registry->AddComponent<WaterComponent>(entity);
        }

        ImGui::EndPopup();
    }
}

void ComponentPanel::ShowAssetPopup(std::shared_ptr<Registry> registry)
{
    static std::string selectedPath = "";

    if (OpenModelAssetPopup || OpenShapeAssetPopup || OpenTextureAssetPopup || OpenAnimationAssetPopup)
    {
        if (OpenAnimationAssetPopup)
            PreviewRenderer::animationPreviewType = AnimationPreviewType::ALL_ANIMATION;

        ImGui::OpenPopup("AssetPopup");

        ImVec2 nextWindowSize = ImVec2(600, 600);
        ImVec2 nextWindowPos = ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x - nextWindowSize.x) / 2, ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y - nextWindowSize.y) / 2);

        ImGui::SetNextWindowPos(nextWindowPos);
        ImGui::SetNextWindowSize(nextWindowSize);

        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        if (ImGui::BeginPopupModal("AssetPopup", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
        {
            auto popupWindowSize = ImGui::GetWindowSize();
            auto previewManager = PreviewManager::Instance();
            auto textureManager = TextureManager::Instance();

            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.22f, 0.22f, 0.22f, 1.f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
            if (ImGui::BeginChild("Cards##AssetPopup", ImVec2(ImVec2(popupWindowSize.x, popupWindowSize.y * 0.85)), true))
            {
                int counter = 0;
                auto& previews = OpenModelAssetPopup ? previewManager->RefModelPreviews()
                               : OpenShapeAssetPopup ? previewManager->RefShapePreviews() 
                               : OpenAnimationAssetPopup ? previewManager->RefAnimationPreviews()
                               : textureManager->GetAllTexture();
                
                for (auto& preview : previews)
                {
                    if (!preview.second)
                        continue;

                    if (selectedPath == preview.first)
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.66f, 0.66f, 0.12f, 1));
                    else
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1));

                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0));
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 2);

                    bool is_focused = false;
                    std::string name = std::filesystem::path(preview.first).filename().string();
                    if (ImGui::BeginChild(preview.first.c_str(), ImVec2(140, 180), true))
                    {
                        is_focused = ImGui::IsWindowFocused();

                        ImGui::Image((ImTextureID)preview.second->GetTextureID(), ImVec2(140, 140), ImVec2(0, 1), ImVec2(1, 0));
                        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(name.c_str()).x) * 0.5f);
                        ImGui::SetCursorPosY(140 + (40 - ImGui::CalcTextSize(name.c_str()).y) * 0.5f);
                        ImGui::Text(name.c_str());
                    }

                    ImGui::EndChild();
                    ImGui::PopStyleVar(2);
                    ImGui::PopStyleColor();

                    if ((counter + 1) % 4 != 0)
                        ImGui::SameLine();

                    if (is_focused)
                    {
                        selectedPath = preview.first;
                    }

                    counter++;
                }
            }

            ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();

            //-------------------------------------------------------
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.85, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0.65, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0.45, 0, 1));
            if (ImGui::Button("OK", ImVec2(195, 60)))
            {
                if (OpenModelAssetPopup && selectedPath != "")
                {
                    auto activeEntity = registry->GetActiveEntity();
                    auto& modelComponent = registry->GetComponent<ModelComponent>(activeEntity);
                    modelComponent.model = ModelManager::Instance()->LoadModel(selectedPath);
                    registry->SetFlag<ModelComponent>(activeEntity, UPDATE_FLAG);
                }
                else if (OpenShapeAssetPopup && selectedPath != "")
                {
                    auto activeEntity = registry->GetActiveEntity();
                    auto& shapeComponent = registry->GetComponent<ShapeComponent>(activeEntity);
                    shapeComponent.shape = ResourceManager::Instance()->GetGeometry(selectedPath);
                    registry->SetFlag<ShapeComponent>(activeEntity, UPDATE_FLAG);
                }
                else if (OpenTextureAssetPopup && selectedPath != "" && textureAssetType == TextureAssetType::DIFFUSE)
                {
                    auto activeEntity = registry->GetActiveEntity();
                    auto& materialComponent = registry->GetComponent<MaterialComponent>(activeEntity);
                    materialComponent.diffuse = TextureManager::Instance()->GetImageTexture(selectedPath);
                    registry->SetFlag<MaterialComponent>(activeEntity, UPDATE_FLAG);
                }
                else if (OpenTextureAssetPopup && selectedPath != "" && textureAssetType == TextureAssetType::NORMAL)
                {
                    auto activeEntity = registry->GetActiveEntity();
                    auto& materialComponent = registry->GetComponent<MaterialComponent>(activeEntity);
                    materialComponent.normal = TextureManager::Instance()->GetImageTexture(selectedPath);
                    registry->SetFlag<MaterialComponent>(activeEntity, UPDATE_FLAG);
                }
                else if (OpenTextureAssetPopup && selectedPath != "" && textureAssetType == TextureAssetType::SPECULAR)
                {
                    auto activeEntity = registry->GetActiveEntity();
                    auto& materialComponent = registry->GetComponent<MaterialComponent>(activeEntity);
                    materialComponent.specular = TextureManager::Instance()->GetImageTexture(selectedPath);
                    registry->SetFlag<MaterialComponent>(activeEntity, UPDATE_FLAG);
                }
                else if (OpenTextureAssetPopup && selectedPath != "" && textureAssetType == TextureAssetType::DUDV)
                {
                    auto activeEntity = registry->GetActiveEntity();
                    auto& waterComponent = registry->GetComponent<WaterComponent>(activeEntity);
                    waterComponent.dudv = TextureManager::Instance()->GetImageTexture(selectedPath);
                    registry->SetFlag<WaterComponent>(activeEntity, UPDATE_FLAG);
                }
                else if (OpenAnimationAssetPopup && selectedPath != "")
                {
                    auto activeEntity = registry->GetActiveEntity();
                    auto& animationComponent = registry->GetComponent<AnimationComponent>(activeEntity);
                    animationComponent.animation = ModelManager::Instance()->LoadAnimation(selectedPath);
                    registry->SetFlag<AnimationComponent>(activeEntity, UPDATE_FLAG);
                }

                if(OpenAnimationAssetPopup)
                    PreviewRenderer::animationPreviewType = AnimationPreviewType::NONE;

                selectedPath = "";
                OpenModelAssetPopup = false;
                OpenShapeAssetPopup = false;
                OpenTextureAssetPopup = false;
                OpenAnimationAssetPopup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(3);

            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0.85, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0.65, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0.45, 1));
            if (ImGui::Button("Load", ImVec2(195, 60)))
            {
                if (OpenAnimationAssetPopup)
                    PreviewRenderer::animationPreviewType = AnimationPreviewType::NONE;

                selectedPath = "";
                OpenModelAssetPopup = false;
                OpenShapeAssetPopup = false;
                OpenTextureAssetPopup = false;
                OpenAnimationAssetPopup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));
            if (ImGui::Button("Cancel", ImVec2(195, 60)))
            {
                if (OpenAnimationAssetPopup)
                    PreviewRenderer::animationPreviewType = AnimationPreviewType::NONE;

                selectedPath = "";
                OpenModelAssetPopup = false;
                OpenShapeAssetPopup = false;
                OpenTextureAssetPopup = false;
                OpenAnimationAssetPopup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(3);

            ImGui::EndPopup();
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }
}