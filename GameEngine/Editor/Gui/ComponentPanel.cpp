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

        if (activeEntity != null)
        {
            if (registry->HasComponent<TagComponent>(activeEntity))
                RenderTagComponent(registry, activeEntity);

		    if (registry->HasComponent<TransformComponent>(activeEntity))
			    RenderTransformComponent(registry, activeEntity);

            if (registry->HasComponent<CameraComponent>(activeEntity))
                RenderCameraComponent(registry, activeEntity);

            if (registry->HasComponent<ShapeComponent>(activeEntity))
                RenderShapeComponent(registry, activeEntity);

            if (registry->HasComponent<ModelComponent>(activeEntity))
                RenderModelComponent(registry, activeEntity);

            if (registry->HasComponent<AnimationComponent>(activeEntity))
                RenderAnimationComponent(registry, activeEntity);

            if (registry->HasComponent<MaterialComponent>(activeEntity))
                RenderMaterialComponent(registry, activeEntity);

            if (registry->HasComponent<BoxColliderComponent>(activeEntity))
                RenderBoxColliderComponent(scene, activeEntity);

            if (registry->HasComponent<SphereColliderComponent>(activeEntity))
                RenderSphereColliderComponent(scene, activeEntity);

            if (registry->HasComponent<ConvexColliderComponent>(activeEntity))
                RenderConvexColliderComponent(scene, activeEntity);

            if (registry->HasComponent<MeshColliderComponent>(activeEntity))
                RenderMeshColliderComponent(scene, activeEntity);

            if (registry->HasComponent<RigidbodyStaticComponent>(activeEntity))
                RenderStaticRigidbodyComponent(scene, activeEntity);

            if (registry->HasComponent<RigidbodyDynamicComponent>(activeEntity))
                RenderDynamicRigidbodyComponent(scene, activeEntity);

            if (registry->HasComponent<DirlightComponent>(activeEntity))
                RenderDirlightComponent(registry, activeEntity);
        
            if (registry->HasComponent<PointLightComponent>(activeEntity))
                RenderPointLightComponent(registry, activeEntity);

            if (registry->HasComponent<SpotLightComponent>(activeEntity))
                RenderSpotLightComponent(registry, activeEntity);

            if (registry->HasComponent<WaterComponent>(activeEntity))
                RenderWaterComponent(registry, activeEntity);

            if (registry->HasComponent<ScriptComponent>(activeEntity))
                RenderScriptComponent(registry, activeEntity);

            RenderAddComponentPopUp(registry, activeEntity);
        }
	}

	ImGui::End();
}

void ComponentPanel::RenderCameraComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto& component = registry->GetComponent<CameraComponent>(entity);
    static bool visible = true;

    if (ImGui::CollapsingHeader(TITLE_CP(std::string(ICON_FA_VIDEO) + " CameraComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 4.f;
        auto& cameraComponent = CameraSystem::GetMainCamera(registry);

        if (&cameraComponent == &component)
        {
            ImGui::BeginDisabled();
        }

        ImGui::Text("Is Main");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox(TITLE_CP("##isMain"), &component.isMain))
        {
            if(&cameraComponent != &component && component.isMain == true)
                cameraComponent.isMain = false;
            
            registry->SetFlag<CameraComponent>(entity, UPDATE_FLAG);
        }

        if (&cameraComponent == &component)
        {
            ImGui::EndDisabled();
        }

        ImGui::Text("User Move");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox(TITLE_CP("##User Move"), &component.enableUserMovement))
        {
            registry->SetFlag<CameraComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Speed");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::DragFloat(TITLE_CP("##Speed"), &component.speed, 0.05f))
        {
            registry->SetFlag<CameraComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Sensitivity");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::DragFloat(TITLE_CP("##Sensitivity"), &component.sensitivity, 0.05f))
        {
            registry->SetFlag<CameraComponent>(entity, UPDATE_FLAG);
        }
        
        ImGui::Text("Fov");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::DragFloat(TITLE_CP("##Fov"), &component.fov, 0.05f, 0.f, 180.f))
        {
            registry->SetFlag<CameraComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("NearPlane");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::DragFloat(TITLE_CP("##NearPlane"), &component.nearPlane, 0.05f))
        {
            registry->SetFlag<CameraComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("FarPlane");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::DragFloat(TITLE_CP("##FarPlane"), &component.farPlane, 0.05f))
        {
            registry->SetFlag<CameraComponent>(entity, UPDATE_FLAG);
        }
    }

    if (visible == false)
        registry->RemoveComponent<CameraComponent>(entity);

    visible = true;
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
                        registry->SetFlag<TransformComponent>(entity, TC_SCALECHANGED_FLAG);
                    }

                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::DragFloat("##ScaleX", &component.scale.x, 0.075f))
                    {
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                        registry->SetFlag<TransformComponent>(entity, TC_SCALECHANGED_FLAG);
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
                        registry->SetFlag<TransformComponent>(entity, TC_SCALECHANGED_FLAG);
                    }

                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::DragFloat("##ScaleY", &component.scale.y, 0.075f))
                    {
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                        registry->SetFlag<TransformComponent>(entity, TC_SCALECHANGED_FLAG);
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
                        registry->SetFlag<TransformComponent>(entity, TC_SCALECHANGED_FLAG);
                    }

                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::DragFloat("##ScaleZ", &component.scale.z, 0.05f))
                    {
                        registry->SetFlag<TransformComponent>(entity, UPDATE_FLAG);
                        registry->SetFlag<TransformComponent>(entity, TC_SCALECHANGED_FLAG);
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
    static auto noTexture = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/NoTexture.png");
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


        //Diffuse Texture
        ImGui::Text("Diffuse");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));

        float imgWidth = glm::min(ImGui::GetContentRegionAvail().x - 10, 512.f);

        GLuint diffuseTextureID = component.diffuse != nullptr ? component.diffuse->GetTextureID() : noTexture->GetTextureID();
        if (ImGui::ImageButton(TITLE_CP("##DiffuseTexture##MaterialComponent"), (ImTextureID)diffuseTextureID, ImVec2(imgWidth, imgWidth), ImVec2(0, 1), ImVec2(1, 0)))
        {
            textureAssetType = TextureAssetType::DIFFUSE;
            OpenTextureAssetPopup = true;
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Image"))
            {
                std::string droppedFilePath((const char*)payload->Data, payload->DataSize);
                component.diffuse = TextureManager::Instance()->LoadImageTexture(droppedFilePath);
                registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));

        if (ImGui::Button("X##DiffuseMaterialTexture", ImVec2(imgWidth, 18)))
        {
            component.diffuse = nullptr;
            registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
        }

        ImGui::PopStyleColor(3);

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

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Image"))
            {
                std::string droppedFilePath((const char*)payload->Data, payload->DataSize);
                component.normal = TextureManager::Instance()->LoadImageTexture(droppedFilePath);
                registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));

        if (ImGui::Button("X##NormalMaterialTexture", ImVec2(imgWidth, 18)))
        {
            component.normal = nullptr;
            registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
        }

        ImGui::PopStyleColor(3);

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

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Image"))
            {
                std::string droppedFilePath((const char*)payload->Data, payload->DataSize);
                component.specular = TextureManager::Instance()->LoadImageTexture(droppedFilePath);
                registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));

        if (ImGui::Button("X##SpecularMaterialTexture", ImVec2(imgWidth, 18)))
        {
            component.specular = nullptr;
            registry->SetFlag<MaterialComponent>(entity, UPDATE_FLAG);
        }

        ImGui::PopStyleColor(3);

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
        if (ImGui::DragFloat(TITLE_CP("##Strength##PointLightComponent"), &component.strength, 0.005f, 0.f, 1000.f))
        {
            registry->SetFlag<PointLightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Radius");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat(TITLE_CP("##Radius##PointLightComponent"), &component.farPlane, 0.05f, 0.1f, 1000.f))
        {
            registry->SetFlag<PointLightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("WeakenDist");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat(TITLE_CP("##WeakenDist##PointLightComponent"), &component.weakenDistance, 0.05f, 0.f, 1000.f))
        {
            registry->SetFlag<PointLightComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("UpdateFreq");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragInt(TITLE_CP("##UpdateFrequency##PointLightComponent"), &component.updateFrequency, 1, 1, 100))
        {
            if (component.updateFrequency == 0)
                component.updateFrequency = 1;

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

        ImGui::Text("UpdateFreq");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragInt(TITLE_CP("##UpdateFrequency##SpotLightComponent"), &component.updateFrequency, 1, 1, 120))
        {
            if (component.updateFrequency == 0)
                component.updateFrequency = 1;

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

    if (visible == false)
        registry->RemoveComponent<SpotLightComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderShapeComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto previewManager = PreviewManager::Instance();
    auto& component = registry->GetComponent<ShapeComponent>(entity);
    static auto noTexture = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/NoTexture.png");

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

        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));

        if (ImGui::Button("X##ShapeTexture##ShapeComponent", ImVec2(imgWidth, 18)))
        {
            component.shape = nullptr;
            registry->SetFlag<ShapeComponent>(entity, UPDATE_FLAG);
        }

        ImGui::PopStyleColor(3);
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
        if (ImGui::DragFloat2(TITLE_CP("##DudvScale##WaterComponent"), &component.dudvScale.x, 0.01f, 0, 1000000));
        {
            registry->SetFlag<WaterComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Wave Strength");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat2(TITLE_CP("##WaveStrength##WaterComponent"), &component.dudvWaveStrength.x, 0.001f, 0.f, 2.f))
        {
            registry->SetFlag<WaterComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Wave Speed");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragFloat(TITLE_CP("##Wave Speed##WaterComponent"), &component.dudvMoveSpeed, 0.001f, 0.f, 1.f))
        {
            registry->SetFlag<WaterComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("UpdateFreq");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::DragInt(TITLE_CP("##UpdateFreq##WaterComponent"), &component.updateFrequency, 1, 1.f, 120.f))
        {
            if (component.updateFrequency == 0)
                component.updateFrequency = 1;

            registry->SetFlag<WaterComponent>(entity, UPDATE_FLAG);
        }

        //DuDv Texture
        {
            static auto noTexture = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/NoTexture.png");
            GLuint textureID = component.dudv != nullptr ? component.dudv->GetTextureID() : noTexture->GetTextureID();

            ImGui::Text("DuDv Texture");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
            float texWidth = glm::min(ImGui::GetContentRegionAvail().x - 10, 512.f);
            if (ImGui::ImageButton((ImTextureID)textureID, ImVec2(texWidth, texWidth)))
            {
                textureAssetType = TextureAssetType::DUDV;
                OpenTextureAssetPopup = true;
            }

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Image"))
                {
                    std::string droppedFilePath((const char*)payload->Data, payload->DataSize);
                    component.dudv = TextureManager::Instance()->LoadImageTexture(droppedFilePath);
                    registry->SetFlag<WaterComponent>(entity, UPDATE_FLAG);
                }
                ImGui::EndDragDropTarget();
            }

            ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));

            if (ImGui::Button("X##DudvTextureWaterCompnent", ImVec2(texWidth, 18)))
            {
                component.dudv = nullptr;
                registry->SetFlag<WaterComponent>(entity, UPDATE_FLAG);
            }

            ImGui::PopStyleColor(3);
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
    static auto noTexture = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/NoTexture.png");

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

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Model"))
            {
                std::string droppedFilePath((const char*)payload->Data, payload->DataSize);

                auto model = ModelManager::Instance()->LoadModel(droppedFilePath);

                if (registry->HasComponent<AnimationComponent>(entity) &&
                    registry->GetComponent<AnimationComponent>(entity).animation &&
                    registry->GetComponent<AnimationComponent>(entity).animation->GetVertexCount() != model->GetVertexCount())
                {
                    LOG_ERROR("ModelComponentPanel", "Loaded model is not assigned to entity's model component! Reason: Loaded animation and model vertex count mismatch!!");
                }
                else
                {
                    component.model = model;
                    registry->SetFlag<ModelComponent>(entity, UPDATE_FLAG);
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));

        if (ImGui::Button("X##ModelTextureModelComponent", ImVec2(imgWidth, 18)))
        {
            component.model = nullptr;
            registry->SetFlag<ModelComponent>(entity, UPDATE_FLAG);
        }

        ImGui::PopStyleColor(3);
    }

    if (visible == false)
        registry->RemoveComponent<ModelComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderAnimationComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    auto previewManager = PreviewManager::Instance();
    auto& component = registry->GetComponent<AnimationComponent>(entity);
    static auto noTexture = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/NoTexture.png");

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("AnimationComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 3.f;

        ImGui::Text("Speed");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::DragFloat("##Speed##AnimationComponent", &component.speed, 0.05f, 0, 5);

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

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Model"))
            {
                std::string droppedFilePath((const char*)payload->Data, payload->DataSize);

                auto model = ModelManager::Instance()->LoadModel(droppedFilePath);
                if (model->hasAnimation)
                {
                    auto animation = ModelManager::Instance()->LoadAnimation(droppedFilePath);

                    if (registry->HasComponent<ModelComponent>(entity) &&
                        registry->GetComponent<ModelComponent>(entity).model &&
                        registry->GetComponent<ModelComponent>(entity).model->GetVertexCount() != animation->GetVertexCount())
                    {
                        LOG_ERROR("AnimationComponentPanel", "Loaded animation is not assigned to entity's animation component! Reason: Loaded model and animation vertex count mismatch!!");
                    }
                    else
                    {
                        component.animation = animation;
                        registry->SetFlag<AnimationComponent>(entity, UPDATE_FLAG | REGENERATE_FLAG);
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));

        if (ImGui::Button("X##AnimationTextureAnimComp", ImVec2(imgWidth, 18)))
        {
            component.animation = nullptr;
            registry->SetFlag<AnimationComponent>(entity, UPDATE_FLAG);
        }

        ImGui::PopStyleColor(3);

        if (component.animation)
            PreviewRenderer::activeAnimationSet.insert(component.animation->GetPath());

        registry->SetFlag<AnimationComponent>(entity, UPDATE_FLAG);
    }

    if (visible == false)
        registry->RemoveComponent<AnimationComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderBoxColliderComponent(std::shared_ptr<Scene> scene, Entity entity)
{
    auto previewManager = PreviewManager::Instance();
    auto registry = scene->GetRegistry();
    auto& component = registry->GetComponent<BoxColliderComponent>(entity);

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("BoxColliderComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 3.f;

        ImGui::Text("Is Trigger");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Is Trigger##BoxColliderComponent", &component.isTrigger))
        {
            registry->SetFlag<BoxColliderComponent>(entity, UPDATE_FLAG);
        }

        //Calculate Automatic Checkbox
        ImGui::Text("Automatic");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Automatic##BoxColliderComponent", &component.calculateAutomatic))
        {
            registry->SetFlag<BoxColliderComponent>(entity, UPDATE_FLAG);
        }

        if(component.calculateAutomatic)
            ImGui::BeginDisabled(true);

        ImGui::Text("Bind To Translate");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Bind To Translate##BoxColliderComponent", &component.bindToTransformTranslate))
        {
            registry->SetFlag<BoxColliderComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Bind To Scale");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Bind To Scale##BoxColliderComponent", &component.bindToTransformScale))
        {
            registry->SetFlag<BoxColliderComponent>(entity, UPDATE_FLAG);
        }

        //Box origion
        ImGui::Text("Origin");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));

        if (ImGui::DragFloat3("##BoxOrigin##BoxColliderComponent", &component.origin.x, 0.05f))
        {
            registry->SetFlag<BoxColliderComponent>(entity, UPDATE_FLAG);
        }

        //Box half sizes
        ImGui::Text("Size");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));

        if (ImGui::DragFloat3("##BoxSize##BoxColliderComponent", &component.halfExtents.x, 0.05f))
        {
            registry->SetFlag<BoxColliderComponent>(entity, UPDATE_FLAG);
        }

        if (component.calculateAutomatic)
            ImGui::EndDisabled();
    }

    if (visible == false)
    {
        registry->RemoveComponent<BoxColliderComponent>(entity);
    }

    visible = true;
}

void ComponentPanel::RenderSphereColliderComponent(std::shared_ptr<Scene> scene, Entity entity)
{
    auto previewManager = PreviewManager::Instance();
    auto registry = scene->GetRegistry();
    auto& component = registry->GetComponent<SphereColliderComponent>(entity);

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("SphereColliderComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 3.f;

        ImGui::Text("Is Trigger");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Is Trigger##SphereColliderComponent", &component.isTrigger))
        {
            registry->SetFlag<SphereColliderComponent>(entity, UPDATE_FLAG);
        }

        //Calculate Automatic Checkbox
        ImGui::Text("Automatic");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Automatic##SphereColliderComponent", &component.calculateAutomatic))
        {
            registry->SetFlag<SphereColliderComponent>(entity, UPDATE_FLAG);
        }

        if (component.calculateAutomatic)
            ImGui::BeginDisabled(true);

        ImGui::Text("Bind To Translate");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Bind To Translate##SphereColliderComponent", &component.bindToTransformTranslate))
        {
            registry->SetFlag<SphereColliderComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Bind To Scale");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Bind To Scale##SphereColliderComponent", &component.bindToTransformScale))
        {
            registry->SetFlag<SphereColliderComponent>(entity, UPDATE_FLAG);
        }

        //Box origion
        ImGui::Text("Origin");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));

        if (ImGui::DragFloat3("##SphereOrigin##SphereColliderComponent", &component.origin.x, 0.05f))
        {
            registry->SetFlag<SphereColliderComponent>(entity, UPDATE_FLAG);
        }

        //Box half sizes
        ImGui::Text("Size");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));

        if (ImGui::DragFloat("##SphereSize##SphereColliderComponent", &component.radius, 0.05f))
        {
            registry->SetFlag<SphereColliderComponent>(entity, UPDATE_FLAG);
        }

        if (component.calculateAutomatic)
            ImGui::EndDisabled();
    }

    if (visible == false)
        registry->RemoveComponent<SphereColliderComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderConvexColliderComponent(std::shared_ptr<Scene> scene, Entity entity)
{
    auto previewManager = PreviewManager::Instance();
    auto registry = scene->GetRegistry();
    auto& component = registry->GetComponent<ConvexColliderComponent>(entity);

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("ConvexColliderComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 3.f;

        ImGui::Text("Is Trigger");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Is Trigger##ConvexColliderComponent", &component.isTrigger))
        {
            registry->SetFlag<ConvexColliderComponent>(entity, UPDATE_FLAG);
        }
    }

    if (visible == false)
        registry->RemoveComponent<ConvexColliderComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderMeshColliderComponent(std::shared_ptr<Scene> scene, Entity entity)
{
    auto previewManager = PreviewManager::Instance();
    auto registry = scene->GetRegistry();
    auto& component = registry->GetComponent<MeshColliderComponent>(entity);

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("MeshColliderComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 3.f;

        ImGui::Text("Is Trigger");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Is Trigger##MeshColliderComponent", &component.isTrigger))
        {
            registry->SetFlag<MeshColliderComponent>(entity, UPDATE_FLAG);
        }
    }

    if (visible == false)
        registry->RemoveComponent<MeshColliderComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderStaticRigidbodyComponent(std::shared_ptr<Scene> scene, Entity entity)
{
    auto previewManager = PreviewManager::Instance();
    auto registry = scene->GetRegistry();
    auto& component = registry->GetComponent<RigidbodyStaticComponent>(entity);

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("RigidbodyStaticComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 3.f;

        ImGui::Text("Static Friction");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::DragFloat("##StaticFriction##RigidbodyStaticComponent", &component.sFriction))
        {
            registry->SetFlag<RigidbodyStaticComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Dynamic Friction");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::DragFloat("##DynamicFriction##RigidbodyStaticComponent", &component.dFriction))
        {
            registry->SetFlag<RigidbodyStaticComponent>(entity, UPDATE_FLAG);
        }

        //Calculate Automatic Checkbox
        ImGui::Text("Restitution");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::DragFloat("##Restitution##RigidbodyStaticComponent", &component.restitution))
        {
            registry->SetFlag<RigidbodyStaticComponent>(entity, UPDATE_FLAG);
        }
    }

    if (visible == false)
    {
        if (scene->gScene)
            scene->gScene->removeActor(*component.staticActor);
        registry->RemoveComponent<RigidbodyStaticComponent>(entity);
    }

    visible = true;
}

void ComponentPanel::RenderDynamicRigidbodyComponent(std::shared_ptr<Scene> scene, Entity entity)
{
    auto previewManager = PreviewManager::Instance();
    auto registry = scene->GetRegistry();
    auto& component = registry->GetComponent<RigidbodyDynamicComponent>(entity);

    static bool visible = true;
    if (ImGui::CollapsingHeader(TITLE_CP("RigidbodyDynamicComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 3.f;

        ImGui::Text("Mass");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::DragFloat("##Mass##RigidbodyDynamicComponent", &component.mass))
        {
            registry->SetFlag<RigidbodyDynamicComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Static Friction");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::DragFloat("##StaticFriction##RigidbodyDynamicComponent", &component.sFriction))
        {
            registry->SetFlag<RigidbodyDynamicComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Dynamic Friction");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::DragFloat("##DynamicFriction##RigidbodyDynamicComponent", &component.dFriction))
        {
            registry->SetFlag<RigidbodyDynamicComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Restitution");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::DragFloat("##Restitution##RigidbodyDynamicComponent", &component.restitution))
        {
            registry->SetFlag<RigidbodyDynamicComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Disable Gravity");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Disable Gravity##RigidbodyDynamicComponent", &component.disableGravity))
        {
            registry->SetFlag<RigidbodyDynamicComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Is Kinematic");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##IsKinematic##RigidbodyDynamicComponent", &component.isKinematic))
        {
            registry->SetFlag<RigidbodyDynamicComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Lock Rotation X");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Lock Rotation X##RigidbodyDynamicComponent", &component.lockRotation[0]))
        {
            registry->SetFlag<RigidbodyDynamicComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Lock Rotation Y");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Lock Rotation Y##RigidbodyDynamicComponent", &component.lockRotation[1]))
        {
            registry->SetFlag<RigidbodyDynamicComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Lock Rotation Z");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Lock Rotation Z##RigidbodyDynamicComponent", &component.lockRotation[2]))
        {
            registry->SetFlag<RigidbodyDynamicComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Lock Position X");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Lock Position X##RigidbodyDynamicComponent", &component.lockPosition[0]))
        {
            registry->SetFlag<RigidbodyDynamicComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Lock Position Y");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Lock Position Y##RigidbodyDynamicComponent", &component.lockPosition[1]))
        {
            registry->SetFlag<RigidbodyDynamicComponent>(entity, UPDATE_FLAG);
        }

        ImGui::Text("Lock Position Z");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
        if (ImGui::Checkbox("##Lock Position Z##RigidbodyDynamicComponent", &component.lockPosition[2]))
        {
            registry->SetFlag<RigidbodyDynamicComponent>(entity, UPDATE_FLAG);
        }
    }

    if (visible == false)
    {
        if (scene->gScene && component.dynamicActor)
            scene->gScene->removeActor(*component.dynamicActor);
        registry->RemoveComponent<RigidbodyDynamicComponent>(entity);
    }

    visible = true;
}

void ComponentPanel::RenderScriptComponent(std::shared_ptr<Registry> registry, Entity entity)
{
    static std::vector<std::string> deleteScripts;
    static std::vector<char> addScriptName(100);
    auto& component = registry->GetComponent<ScriptComponent>(entity);
    static bool visible = true;

    if (ImGui::CollapsingHeader(TITLE_CP(std::string(ICON_FA_SCROLL) + " ScriptComponent"), &visible, ImGuiTreeNodeFlags_DefaultOpen))
    {
        float width = ImGui::GetContentRegionAvail().x / 4.f;
     
        ImGui::PushItemWidth(width * 3);
        ImGui::InputText(TITLE_CP("##AddScriptInputText"), addScriptName.data(), addScriptName.size());
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.85, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0.65, 0, 1));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0.45, 0, 1));
        if (ImGui::Button("Add Script", ImVec2(ImGui::GetContentRegionAvail().x, 18)))
        {
            std::string scriptName(addScriptName.begin(), addScriptName.end());

            if (scriptName != "" && component.scripts.find(scriptName) == component.scripts.end())
            {
                component.scripts.insert(std::make_pair(scriptName, nullptr));
            }

            std::fill(addScriptName.begin(), addScriptName.end(), '\0');
        }
        ImGui::PopStyleColor(3);

        if (ImGui::BeginTable(TITLE_CP("Loaded Scripts Table"), 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders))
        {
            ImGui::TableSetupColumn("Loaded Scripts", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Delete", ImGuiTableColumnFlags_WidthFixed, 50.f);
            ImGui::TableHeadersRow();

            for (auto& [name, script] : component.scripts)
            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text(name.c_str());

                ImGui::TableSetColumnIndex(1);

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));
                if (ImGui::Button(TITLE_CP("X##" + name), ImVec2(50, 20)))
                {
                    deleteScripts.push_back(name);
                }

                ImGui::PopStyleColor(3);
            }

            ImGui::EndTable();
        }
    }

    for (auto& scriptName : deleteScripts)
    {
        if(component.scripts[scriptName])
            delete component.scripts[scriptName];

        component.scripts.erase(scriptName);
    }
    deleteScripts.clear();

    if (visible == false)
        registry->RemoveComponent<ScriptComponent>(entity);

    visible = true;
}

void ComponentPanel::RenderAddComponentPopUp(std::shared_ptr<Registry> registry, Entity entity)
{
    if (ImGui::BeginPopupContextWindow())
    {
        ImGui::SeparatorText("General");

        bool enabledTagComponent = !registry->HasComponent<TagComponent>(entity);
        if (ImGui::MenuItem(TITLE_CP("Tag Component"), NULL, registry->HasComponent<TagComponent>(entity), enabledTagComponent))
        {
            if (!registry->HasComponent<TagComponent>(entity))
                registry->AddComponent<TagComponent>(entity);
        }

        bool enabledTransformComponent = !registry->HasComponent<TransformComponent>(entity);
        if (ImGui::MenuItem(TITLE_CP("Transform Component"), NULL, registry->HasComponent<TransformComponent>(entity), enabledTransformComponent))
        {
            if (!registry->HasComponent<TransformComponent>(entity))
                registry->AddComponent<TransformComponent>(entity);
        }

        bool enabledCameraComponent = !registry->HasComponent<CameraComponent>(entity);
        if (ImGui::MenuItem(TITLE_CP("Camera Component"), NULL, registry->HasComponent<CameraComponent>(entity), enabledCameraComponent))
        {
            if (!registry->HasComponent<CameraComponent>(entity))
                registry->AddComponent<CameraComponent>(entity);
        }

        ImGui::SeparatorText("Objects");

        bool enabledShapeComponent = !registry->HasComponent<ShapeComponent>(entity) && !registry->HasComponent<ModelComponent>(entity) && !registry->HasComponent<AnimationComponent>(entity);
        if (ImGui::MenuItem(TITLE_CP("Shape Component"), NULL, registry->HasComponent<ShapeComponent>(entity), enabledShapeComponent))
        {
            if (!registry->HasComponent<ShapeComponent>(entity))
                registry->AddComponent<ShapeComponent>(entity);
        }

        bool enabledModelComponent = !registry->HasComponent<ModelComponent>(entity) && !registry->HasComponent<ShapeComponent>(entity) && !registry->HasComponent<MaterialComponent>(entity);
        if (ImGui::MenuItem(TITLE_CP("Model Component"), NULL, registry->HasComponent<ModelComponent>(entity), enabledModelComponent))
        {
            if (!registry->HasComponent<ModelComponent>(entity))
                registry->AddComponent<ModelComponent>(entity);
        }

        bool enabledAnimationComponent = !registry->HasComponent<AnimationComponent>(entity) && !registry->HasComponent<ShapeComponent>(entity) && !registry->HasComponent<MaterialComponent>(entity);
        if (ImGui::MenuItem(TITLE_CP("Animation Component"), NULL, registry->HasComponent<AnimationComponent>(entity), enabledAnimationComponent))
        {
            if (!registry->HasComponent<AnimationComponent>(entity))
                registry->AddComponent<AnimationComponent>(entity);
        }

        bool enabledMaterialComponent = !registry->HasComponent<MaterialComponent>(entity) && !registry->HasComponent<ModelComponent>(entity) && !registry->HasComponent<AnimationComponent>(entity);
        if (ImGui::MenuItem(TITLE_CP("Material Component"), NULL, registry->HasComponent<MaterialComponent>(entity), enabledMaterialComponent))
        {
            if (!registry->HasComponent<MaterialComponent>(entity))
                registry->AddComponent<MaterialComponent>(entity);
        }

        ImGui::SeparatorText("Lights");

        bool enabledLightsComponent = !registry->HasComponent<DirlightComponent>(entity) && !registry->HasComponent<PointLightComponent>(entity) && !registry->HasComponent<SpotLightComponent>(entity);
        if (ImGui::MenuItem(TITLE_CP("Dirlight Component"), NULL, registry->HasComponent<DirlightComponent>(entity), enabledLightsComponent))
        {
            if (!registry->HasComponent<DirlightComponent>(entity))
                registry->AddComponent<DirlightComponent>(entity);
        }
    
        if (ImGui::MenuItem(TITLE_CP("Pointlight Component"), NULL, registry->HasComponent<PointLightComponent>(entity), enabledLightsComponent))
        {
            if (!registry->HasComponent<PointLightComponent>(entity))
                registry->AddComponent<PointLightComponent>(entity);
        }

        if (ImGui::MenuItem(TITLE_CP("Spotlight Component"), NULL, registry->HasComponent<SpotLightComponent>(entity), enabledLightsComponent))
        {
            if (!registry->HasComponent<SpotLightComponent>(entity))
                registry->AddComponent<SpotLightComponent>(entity);
        }

        ImGui::SeparatorText("Colliders");

        bool enabledColliderComponent = !registry->HasComponent<BoxColliderComponent>(entity) && !registry->HasComponent<SphereColliderComponent>(entity) && !registry->HasComponent<ConvexColliderComponent>(entity) && !registry->HasComponent<MeshColliderComponent>(entity);
        
        if (ImGui::MenuItem(TITLE_CP("BoxCollider Component"), NULL, registry->HasComponent<BoxColliderComponent>(entity), enabledColliderComponent))
        {
            if (!registry->HasComponent<BoxColliderComponent>(entity))
                registry->AddComponent<BoxColliderComponent>(entity);
        }

        if (ImGui::MenuItem(TITLE_CP("SphereCollider Component"), NULL, registry->HasComponent<SphereColliderComponent>(entity), enabledColliderComponent))
        {
            if (!registry->HasComponent<SphereColliderComponent>(entity))
                registry->AddComponent<SphereColliderComponent>(entity);
        }

        if (ImGui::MenuItem(TITLE_CP("ConvexCollider Component"), NULL, registry->HasComponent<ConvexColliderComponent>(entity), enabledColliderComponent))
        {
            if (!registry->HasComponent<ConvexColliderComponent>(entity))
                registry->AddComponent<ConvexColliderComponent>(entity);
        }

        if (ImGui::MenuItem(TITLE_CP("MeshCollider Component"), NULL, registry->HasComponent<MeshColliderComponent>(entity), enabledColliderComponent))
        {
            if (!registry->HasComponent<MeshColliderComponent>(entity))
                registry->AddComponent<MeshColliderComponent>(entity);
        }

        ImGui::SeparatorText("Physics");

        bool enabledPhysicsComponent = !registry->HasComponent<RigidbodyStaticComponent>(entity) && !registry->HasComponent<RigidbodyDynamicComponent>(entity);

        if (ImGui::MenuItem(TITLE_CP("Static Rigidbody Component"), NULL, registry->HasComponent<RigidbodyStaticComponent>(entity), enabledPhysicsComponent))
        {
            if (!registry->HasComponent<RigidbodyStaticComponent>(entity))
                registry->AddComponent<RigidbodyStaticComponent>(entity);
        }

        if (ImGui::MenuItem(TITLE_CP("Dynamic Rigidbody Component"), NULL, registry->HasComponent<RigidbodyDynamicComponent>(entity), enabledPhysicsComponent))
        {
            if (!registry->HasComponent<RigidbodyDynamicComponent>(entity))
                registry->AddComponent<RigidbodyDynamicComponent>(entity);
        }

        ImGui::SeparatorText("Other");

        bool enabledWaterComponent = !registry->HasComponent<WaterComponent>(entity);

        if (ImGui::MenuItem(TITLE_CP("Water Component"), NULL, registry->HasComponent<WaterComponent>(entity), enabledWaterComponent))
        {
            if (!registry->HasComponent<WaterComponent>(entity))
                registry->AddComponent<WaterComponent>(entity);
        }

        bool enabledScriptComponent = !registry->HasComponent<ScriptComponent>(entity);

        if (ImGui::MenuItem(TITLE_CP("Script Component"), NULL, registry->HasComponent<ScriptComponent>(entity), enabledScriptComponent))
        {
            if (!registry->HasComponent<ScriptComponent>(entity))
                registry->AddComponent<ScriptComponent>(entity);
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
        {
            for (auto& [path, animation] : ModelManager::Instance()->GetAnimationList())
            {
                if(animation)
                    PreviewRenderer::activeAnimationSet.insert(animation->GetPath());
            }
        }

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

                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                        ImGui::Image((ImTextureID)preview.second->GetTextureID(), ImVec2(140, 140), ImVec2(0, 1), ImVec2(1, 0));
                        ImGui::PopStyleVar();
                        
                        ImGui::Separator();
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
                    auto model = ModelManager::Instance()->LoadModel(selectedPath);

                    if (registry->HasComponent<AnimationComponent>(activeEntity) &&
                        registry->GetComponent<AnimationComponent>(activeEntity).animation &&
                        registry->GetComponent<AnimationComponent>(activeEntity).animation->GetVertexCount() != model->GetVertexCount())
                    {
                        LOG_ERROR("ModelComponentPanel", "Loaded model is not assigned to entity's model component! Reason: Loaded animation and model vertex count mismatch!!");
                    }
                    else
                    {
                        modelComponent.model = model;
                        registry->SetFlag<ModelComponent>(activeEntity, UPDATE_FLAG);
                    }
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

                    auto animation = ModelManager::Instance()->LoadAnimation(selectedPath);

                    if (registry->HasComponent<ModelComponent>(activeEntity) &&
                        registry->GetComponent<ModelComponent>(activeEntity).model &&
                        registry->GetComponent<ModelComponent>(activeEntity).model->GetVertexCount() != animation->GetVertexCount())
                    {
                        LOG_ERROR("AnimationComponentPanel", "Loaded animation is not assigned to entity's animation component! Reason: Loaded model and animation vertex count mismatch!!");
                    }
                    else
                    {
                        animationComponent.animation = animation;
                        registry->SetFlag<AnimationComponent>(activeEntity, UPDATE_FLAG);
                    }
                }

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
                if (!OpenShapeAssetPopup)
                {
                    FileDialogOption option;
                    option.dialogType = FileDialogType::OPEN_DIALOG;
                    option.returnType = FileDialogReturnType::PICK_FILE;

                    if (OpenTextureAssetPopup)
                        option.filters.push_back({ L"Texture", L"*.png;*.jpg" });
                    else if (OpenModelAssetPopup)
                        option.filters.push_back({ L"Model", L"*.obj;*.dae;*.fbx" });
                    else if (OpenAnimationAssetPopup)
                        option.filters.push_back({ L"Model", L"*.obj;*.dae;*.fbx" });

                    std::string path = FileDialogWindows::ShowFileDialog(option);

                    if (path != "")
                    {
                        if (OpenTextureAssetPopup && std::filesystem::exists(path) && (std::filesystem::path(path).extension() == ".png" || std::filesystem::path(path).extension() == ".jpg"))
                        {
                            if (textureAssetType == TextureAssetType::DIFFUSE)
                            {
                                auto activeEntity = registry->GetActiveEntity();
                                auto& materialComponent = registry->GetComponent<MaterialComponent>(activeEntity);
                                materialComponent.diffuse = TextureManager::Instance()->LoadImageTexture(path);
                                registry->SetFlag<MaterialComponent>(activeEntity, UPDATE_FLAG);
                            }
                            else if (textureAssetType == TextureAssetType::SPECULAR)
                            {
                                auto activeEntity = registry->GetActiveEntity();
                                auto& materialComponent = registry->GetComponent<MaterialComponent>(activeEntity);
                                materialComponent.specular = TextureManager::Instance()->LoadImageTexture(path);
                                registry->SetFlag<MaterialComponent>(activeEntity, UPDATE_FLAG);
                            }
                            else if (textureAssetType == TextureAssetType::NORMAL)
                            {
                                auto activeEntity = registry->GetActiveEntity();
                                auto& materialComponent = registry->GetComponent<MaterialComponent>(activeEntity);
                                materialComponent.normal = TextureManager::Instance()->LoadImageTexture(path);
                                registry->SetFlag<MaterialComponent>(activeEntity, UPDATE_FLAG);
                            }
                            else if (textureAssetType == TextureAssetType::DUDV)
                            {
                                auto activeEntity = registry->GetActiveEntity();
                                auto& waterComponent = registry->GetComponent<WaterComponent>(activeEntity);
                                waterComponent.dudv = TextureManager::Instance()->LoadImageTexture(path);
                                registry->SetFlag<WaterComponent>(activeEntity, UPDATE_FLAG);
                            }
                        }
                        else if (OpenModelAssetPopup && std::filesystem::exists(path) && (std::filesystem::path(path).extension() == ".obj" || std::filesystem::path(path).extension() == ".fbx" || std::filesystem::path(path).extension() == ".dae"))
                        {
                            auto activeEntity = registry->GetActiveEntity();
                            auto& modelComponent = registry->GetComponent<ModelComponent>(activeEntity);
                            auto model = ModelManager::Instance()->LoadModel(path);

                            if (registry->HasComponent<AnimationComponent>(activeEntity) &&
                                registry->GetComponent<AnimationComponent>(activeEntity).animation &&
                                registry->GetComponent<AnimationComponent>(activeEntity).animation->GetVertexCount() != model->GetVertexCount())
                            {
                                LOG_ERROR("ModelComponentPanel", "Loaded model is not assigned to entity's model component! Reason: Loaded animation and model vertex count mismatch!!");
                            }
                            else
                            {
                                modelComponent.model = model;
                                registry->SetFlag<ModelComponent>(activeEntity, UPDATE_FLAG);
                            }
                        }
                        else if (OpenAnimationAssetPopup && std::filesystem::exists(path) && (std::filesystem::path(path).extension() == ".obj" || std::filesystem::path(path).extension() == ".fbx" || std::filesystem::path(path).extension() == ".dae"))
                        {
                            auto activeEntity = registry->GetActiveEntity();
                            auto& animationComponent = registry->GetComponent<AnimationComponent>(activeEntity);

                            auto animation = ModelManager::Instance()->LoadAnimation(path);

                            if (registry->HasComponent<ModelComponent>(activeEntity) &&
                                registry->GetComponent<ModelComponent>(activeEntity).model &&
                                registry->GetComponent<ModelComponent>(activeEntity).model->GetVertexCount() != animation->GetVertexCount())
                            {
                                LOG_ERROR("AnimationComponentPanel", "Loaded animation is not assigned to entity's animation component! Reason: Loaded model and animation vertex count mismatch!!");
                            }
                            else
                            {
                                animationComponent.animation = animation;
                                registry->SetFlag<AnimationComponent>(activeEntity, UPDATE_FLAG);
                            }
                        }

                        selectedPath = "";
                        OpenModelAssetPopup = false;
                        OpenShapeAssetPopup = false;
                        OpenTextureAssetPopup = false;
                        OpenAnimationAssetPopup = false;
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));
            if (ImGui::Button("Cancel", ImVec2(195, 60)))
            {
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