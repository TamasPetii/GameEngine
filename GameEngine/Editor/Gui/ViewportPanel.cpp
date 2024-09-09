#include "ViewportPanel.h"

void ViewportPanel::Update(std::shared_ptr<Scene> scene)
{
    if (m_ViewportSizeChanged)
    {
        m_ViewportSizeChanged = false;
        auto resourceManager = ResourceManager::Instance();
        resourceManager->GetFbo("Main")->Resize(m_ViewportSize.x, m_ViewportSize.y);

        auto& cameraComponent = CameraSystem::GetMainCamera(scene->GetRegistry());
        cameraComponent.width = m_ViewportSize.x;
        cameraComponent.height = m_ViewportSize.y;

        { //Bloom Framebuffer Resize
            auto fbo = resourceManager->GetFbo("Bloom");
            glm::ivec2 size = resourceManager->GetFbo("Main")->GetSize();
            for (unsigned int i = 0; i < 6; i++)
            {
                std::string name = "bloom" + std::to_string(i);
                glm::ivec2 textureSize = size / 2;

                auto& spec = fbo->GetTextureSpec(name);
                spec.width = textureSize.x;
                spec.height = textureSize.y;

                size = textureSize;
            }

            fbo->Resize(m_ViewportSize.x, m_ViewportSize.y);
        }
    }
}

void ViewportPanel::Render(std::shared_ptr<Scene> scene, float deltaTime)
{
    static std::shared_ptr<TextureGL> play = TextureManager::Instance()->LoadImageTexture("../Assets/play.png");
    static std::shared_ptr<TextureGL> pause = TextureManager::Instance()->LoadImageTexture("../Assets/pause.png");
    static std::shared_ptr<TextureGL> exit = TextureManager::Instance()->LoadImageTexture("../Assets/cross.png");

    static bool is_open = true;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin(TITLE_VP("ViewPort"), &is_open, ImGuiWindowFlags_NoTitleBar))
	{
        ImVec2 size = ImGui::GetContentRegionAvail();
        auto resourceManager = ResourceManager::Instance();
        auto fbo = resourceManager->GetFbo("Main");

        ImGui::Image((ImTextureID)fbo->GetTextureID("main"), size, ImVec2(0, 1), ImVec2(1, 0));

        if (size.x != fbo->GetSize().x || size.y != fbo->GetSize().y)
        {
            m_ViewportSize = size;
            m_ViewportSizeChanged = true;
        }

        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - 50, 16));
        if (ImGui::ImageButton((ImTextureID)play->GetTextureID(), ImVec2(32, 32), ImVec2(0, 1), ImVec2(1, 0)))
        {
            GlobalSettings::GameViewActive = true;
            GlobalSettings::HideCursor = true;
        }

        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2, 16));
        if (ImGui::ImageButton((ImTextureID)pause->GetTextureID(), ImVec2(32, 32), ImVec2(0, 1), ImVec2(1, 0)))
        {

        }

        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 + 50, 16));
        if (ImGui::ImageButton((ImTextureID)exit->GetTextureID(), ImVec2(32, 32), ImVec2(0, 1), ImVec2(1, 0)))
        {
            GlobalSettings::GameViewActive = false;
            GlobalSettings::HideCursor = false;
        }

        RenderFpsCounter(scene, deltaTime);

        if (!GlobalSettings::GameViewActive)
        {
            RenderGizmos(scene);

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                int mouseX = ImGui::GetMousePos().x - ImGui::GetWindowContentRegionMin().x - ImGui::GetWindowPos().x;
                int mouseY = ImGui::GetMousePos().y - ImGui::GetWindowContentRegionMin().y - ImGui::GetWindowPos().y;
                int contentRegionX = ImGui::GetContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
                int contentRegionY = ImGui::GetContentRegionMax().y - ImGui::GetWindowContentRegionMin().y;
                mouseY = contentRegionY - mouseY;

                std::cout << mouseX << " " << mouseY << std::endl;

                if (mouseX >= 0 && mouseX < contentRegionX &&
                    mouseY >= 0 && mouseY < contentRegionY && 
                    ImGui::IsWindowHovered() &&
                    !ImGuizmo::IsUsing())
                {
                    unsigned int id = std::any_cast<unsigned int>(fbo->ReadPixel("id", mouseX, mouseY));
                    scene->GetRegistry()->SetActiveEntity(id);
                    std::cout << "Entity ID: " << id << std::endl;
                }
            }
        }
	}
    ImGui::PopStyleVar();

	ImGui::End();
}

void ViewportPanel::RenderGizmos(std::shared_ptr<Scene> scene)
{
    auto& registry = scene->GetRegistry();
    auto& cameraComponent = CameraSystem::GetMainCamera(registry);
    auto activeEntity = registry->GetActiveEntity();

    if (activeEntity != null && registry->HasComponent<TransformComponent>(activeEntity))
    {
        static ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
        static ImGuizmo::MODE currentMode = ImGuizmo::WORLD;

        glm::mat4 viewMatrix = cameraComponent.view;
        glm::mat4 projectionMatrix = cameraComponent.proj;
        auto& transformComponent = registry->GetComponent<TransformComponent>(activeEntity);
        auto transform = transformComponent.fullTransform;

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        ImGuizmo::SetDrawlist();
        ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), currentOperation, currentMode, glm::value_ptr(transform), NULL, NULL, NULL);

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 translation, scale, rotation;
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));

            glm::vec3 lastTranslation, lastScale, lastRotation;
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transformComponent.fullTransform), glm::value_ptr(lastTranslation), glm::value_ptr(lastRotation), glm::value_ptr(lastScale));

            switch (currentOperation)
            {
            case ImGuizmo::TRANSLATE:
                transformComponent.translate += translation - lastTranslation;
                registry->SetFlag<TransformComponent>(activeEntity, UPDATE_FLAG);
                break;
            case ImGuizmo::ROTATE:
                transformComponent.rotate += rotation - lastRotation;
                registry->SetFlag<TransformComponent>(activeEntity, UPDATE_FLAG);
                break;
            case ImGuizmo::SCALE:
                transformComponent.scale += scale - lastScale;
                registry->SetFlag<TransformComponent>(activeEntity, UPDATE_FLAG);
                break;
            }
        }

        if (ImGui::IsKeyPressed(ImGuiKey_1))
        {
            currentOperation = ImGuizmo::TRANSLATE;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_2))
        {
            currentOperation = ImGuizmo::ROTATE;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_3))
        {
            currentOperation = ImGuizmo::SCALE;
        }
    }
}

void ViewportPanel::RenderFpsCounter(std::shared_ptr<Scene> scene, float deltaTime)
{
    static std::string fps = "0";
    static float time = 0;
    static int count = 0;

    time += deltaTime;
    count += 1;

    if (time > 1)
    {
        fps = std::to_string(count);
        time = 0;
        count = 0;
    }

    float viewPortWidth = ImGui::GetWindowWidth();

    auto font = ImGui::GetFont();
    float prevFontSize = font->FontSize;
    font->FontSize = 8;

    ImGui::SetCursorPos(ImVec2(viewPortWidth - ImGui::CalcTextSize(fps.c_str()).x - 5, 3));
    ImGui::TextColored(ImVec4(1, 0, 0, 1), fps.c_str());

    font->FontSize = prevFontSize;
}