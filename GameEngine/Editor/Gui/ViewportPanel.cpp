#include "ViewportPanel.h"

void ViewportPanel::Update(std::shared_ptr<Scene> scene)
{
    if (m_ViewportSizeChanged)
    {
        m_ViewportSizeChanged = false;
        auto resourceManager = ResourceManager::Instance();
        resourceManager->GetFbo("Main")->Resize(m_ViewportSize.x, m_ViewportSize.y);
        scene->GetMainCamera()->Resize(m_ViewportSize.x, m_ViewportSize.y);

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
        }

        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2, 16));
        if (ImGui::ImageButton((ImTextureID)pause->GetTextureID(), ImVec2(32, 32), ImVec2(0, 1), ImVec2(1, 0)))
        {

        }

        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 + 50, 16));
        if (ImGui::ImageButton((ImTextureID)exit->GetTextureID(), ImVec2(32, 32), ImVec2(0, 1), ImVec2(1, 0)))
        {
            GlobalSettings::GameViewActive = false;
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

        ViewportPanel::CameraKeyboardEvent(scene);
        ViewportPanel::CameraButtonEvent(scene);
	}
    ImGui::PopStyleVar();

	ImGui::End();
}

void ViewportPanel::CameraKeyboardEvent(std::shared_ptr<Scene> scene)
{
    auto& camera = scene->GetMainCamera();

    //W
    if (ImGui::IsKeyPressed(ImGuiKey_W))
    {
        camera->KeyboardDown(GLFW_KEY_W);
    }
    if (ImGui::IsKeyReleased(ImGuiKey_W))
    {
        camera->KeyboardUp(GLFW_KEY_W);
    }

    //S
    if (ImGui::IsKeyPressed(ImGuiKey_S))
    {
        camera->KeyboardDown(GLFW_KEY_S);
    }
    if (ImGui::IsKeyReleased(ImGuiKey_S))
    {
        camera->KeyboardUp(GLFW_KEY_S);
    }

    //A
    if (ImGui::IsKeyPressed(ImGuiKey_A))
    {
        camera->KeyboardDown(GLFW_KEY_A);
    }
    if (ImGui::IsKeyReleased(ImGuiKey_A))
    {
        camera->KeyboardUp(GLFW_KEY_A);
    }

    //D
    if (ImGui::IsKeyPressed(ImGuiKey_D))
    {
        camera->KeyboardDown(GLFW_KEY_D);
    }
    if (ImGui::IsKeyReleased(ImGuiKey_D))
    {
        camera->KeyboardUp(GLFW_KEY_D);
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Space))
    {
        #undef PlaySound

        auto soundManager = SoundManager::Instance();
        auto source = soundManager->LoadSoundSource("../Assets/Awp.wav");
        auto sound = soundManager->PlaySound(source);
    }
}

void ViewportPanel::CameraButtonEvent(std::shared_ptr<Scene> scene)
{
    static bool isMouseClicked = false;
    auto& camera = scene->GetMainCamera();

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        isMouseClicked = true;
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
    {
        isMouseClicked = false;
    }

    if (ImGui::IsWindowHovered())
    {
        static ImVec2 mousePosPrev = ImGui::GetIO().MousePos;
        if (isMouseClicked)
        {
            ImVec2 mousePos = ImGui::GetIO().MousePos;
            camera->MouseMove(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, mousePos.x - mousePosPrev.x, mousePos.y - mousePosPrev.y);
        }
        mousePosPrev = ImGui::GetIO().MousePos;
    }
}

void ViewportPanel::RenderGizmos(std::shared_ptr<Scene> scene)
{
    auto& camera = scene->GetMainCamera();
    auto& registry = scene->GetRegistry();
    auto activeEntity = registry->GetActiveEntity();

    if (activeEntity != null && registry->HasComponent<TransformComponent>(activeEntity))
    {
        static ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
        static ImGuizmo::MODE currentMode = ImGuizmo::WORLD;

        glm::mat4 viewMatrix = camera->GetView();
        glm::mat4 projectionMatrix = camera->GetProj();
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