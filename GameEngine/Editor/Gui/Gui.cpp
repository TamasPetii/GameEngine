#include "Gui.h"

bool Gui::OpenGlobalSettingsPopup = false;
bool Gui::OpenAskSceneSavePopup = false;

void Gui::Update(std::shared_ptr<Scene> scene)
{
    ViewportPanel::Update(scene);
}

void Gui::PreRender()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::PostRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Gui::Render(std::shared_ptr<Scene> scene, float deltaTime)
{
    PreRender();
    RenderDockSpace(scene);

    if(!GlobalSettings::GameViewActive)
        ImGui::ShowDemoWindow();

    FilesystemPanel::Render();
    ConsolePanel::Render();
    ViewportPanel::Render(scene, deltaTime);
    EntitiesPanel::Render(scene);
    ComponentPanel::Render(scene);
    SettingsPanel::Render(scene);
    Gui::RenderScriptGui(scene);

    PostRender();
}

void Gui::RenderDockSpace(std::shared_ptr<Scene> scene)
{
    if (GlobalSettings::UseDockingSpace)
    {
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace", nullptr, window_flags);

        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        RenderMainTitleBar(scene);
        RenderPopupModals(scene);

        ImGui::End();
    }
}

void Gui::RenderMainTitleBar(std::shared_ptr<Scene> scene)
{
    if (!GlobalSettings::GameViewActive)
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Project"))
                {

                }

                ImGui::SeparatorText("Scene");

                if (ImGui::MenuItem("New Scene"))
                {
                    OpenAskSceneSavePopup = true;
                }
                if (ImGui::MenuItem("Load Scene"))
                {
                    FileDialogOption option;
                    option.dialogType = FileDialogType::OPEN_DIALOG;
                    option.returnType = FileDialogReturnType::PICK_FILE;
                    option.filters.push_back({ L"Engine Scene", L"*.json" });
                    std::string path = FileDialogWindows::ShowFileDialog(option);
                    if (std::filesystem::exists(path) && std::filesystem::path(path).extension() == ".json")
                    {
                        scene->DeSerialize(path);
                        ViewportPanel::m_ViewportSizeChanged = true;
                    }
                }
                if (ImGui::MenuItem("Save Scene"))
                {
                    FileDialogOption option;
                    option.dialogType = FileDialogType::SAVE_DIALOG;
                    option.returnType = FileDialogReturnType::PICK_FILE;
                    option.filters.push_back({ L"Engine Scene", L"*.json" });
                    std::string path = FileDialogWindows::ShowFileDialog(option);
                    if (path != "" && std::filesystem::path(path).extension() == ".json")
                        scene->Serialize(path);
                }

                ImGui::SeparatorText("Settings");

                if (ImGui::MenuItem("Settings"))
                {
                    OpenGlobalSettingsPopup = true;
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }
}

void Gui::SetStyle()
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.45f, 0.45f, 0.45f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(8.00f, 8.00f);
    style.FramePadding = ImVec2(5.00f, 2.00f);
    style.CellPadding = ImVec2(6.00f, 6.00f);
    style.ItemSpacing = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.IndentSpacing = 25;
    style.ScrollbarSize = 15;
    style.GrabMinSize = 10;
    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 1;
    style.TabBorderSize = 1;
    style.WindowRounding = 7;
    style.ChildRounding = 4;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ScrollbarRounding = 9;
    style.GrabRounding = 3;
    style.LogSliderDeadzone = 4;
    style.TabRounding = 4;
}


void Gui::RenderPopupModals(std::shared_ptr<Scene> scene)
{
    //This is called in the dockspace window 
    ComponentPanel::ShowAssetPopup(scene->GetRegistry());
    Gui::ShowGlobalSettingsPopup();
    Gui::ShowAskSceneSavePopup(scene);
}

void Gui::ShowGlobalSettingsPopup()
{
    static std::string selectedPath = "";
    static auto noTexture = TextureManager::Instance()->LoadImageTexture("../Assets/NoTexture.png");

    if (OpenGlobalSettingsPopup)
    {
        ImGui::OpenPopup("GlobalSettingsPopup");

        ImVec2 nextWindowSize = ImVec2(400, 600);
        ImVec2 nextWindowPos = ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x - nextWindowSize.x) / 2, ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y - nextWindowSize.y) / 2);

        ImGui::SetNextWindowPos(nextWindowPos);
        ImGui::SetNextWindowSize(nextWindowSize);

        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        if (ImGui::BeginPopupModal("GlobalSettingsPopup", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
        {
            auto popupWindowSize = ImGui::GetWindowSize();
            auto previewManager = PreviewManager::Instance();
            auto textureManager = TextureManager::Instance();

            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.22f, 0.22f, 0.22f, 1.f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
            if (ImGui::BeginChild("Settings##GlobalSettingsPopup", ImVec2(ImVec2(popupWindowSize.x, popupWindowSize.y * 0.85)), true))
            {
                float width = ImGui::GetContentRegionAvail().x / 3.15f;

                ImGui::SeparatorText("Camera");

                ImGui::Text("Hide Cursor");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                ImGui::Checkbox("##Hide Cursor##GlobalSettingsPopup", &GlobalSettings::HideCursor);

                ImGui::SeparatorText("Physics");

                ImGui::Text("Enable in editor");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                ImGui::Checkbox("##Simulate Physics in Editor##GlobalSettingsPopup", &GlobalSettings::EnablePhysicsInEditor);

                ImGui::SeparatorText("Animation");

                ImGui::Text("Enable in editor");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                ImGui::Checkbox("##Simulate Animation in Editor##GlobalSettingsPopup", &GlobalSettings::EnableAnimationInEditor);

                ImGui::SeparatorText("Skybox");

                ImGui::Text("Use Skybox");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                ImGui::Checkbox("##UseSkybox##GlobalSettingsPopup", &GlobalSettings::UseSkybox);

                ImGui::Text("Rotation Speed");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                ImGui::DragFloat3("##Rotation Speed##GlobalSettingsPopup", &GlobalSettings::SkyboxRotationSpeed.x, 0.02f);

                ImGui::Text("Rotation Dir");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                ImGui::SliderInt3("##Rotation Dir##GlobalSettingsPopup", &GlobalSettings::SkyboxRotationDirection.x, -1, 1);

                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                if(ImGui::Button("Reset Rotation##GlobalSettingsPopup", ImVec2(ImGui::GetContentRegionAvail().x - 15, 18)))
                {
                    GlobalSettings::SkyboxRotation = glm::vec3(0, 0, 0);
                }

                ImGui::Text("Skybox Texture");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                auto skyboxTexture = GlobalSettings::SkyboxTexture ? TextureManager::Instance()->LoadImageTexture(GlobalSettings::SkyboxTexture->GetPath()) : noTexture;           

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.4f, 0.4f, 1.f));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3, 3));
                float skyboxTextureWidth = ImGui::GetContentRegionAvail().x - 15;
                if (ImGui::ImageButton((ImTextureID)skyboxTexture->GetTextureID(), ImVec2(skyboxTextureWidth, skyboxTextureWidth), ImVec2(0, 1), ImVec2(1, 0)))
                {
                    FileDialogOption option;
                    option.dialogType = FileDialogType::OPEN_DIALOG;
                    option.returnType = FileDialogReturnType::PICK_FILE;
                    option.filters.push_back({ L"Skybox", L"*.png;*.jpg" });
                    std::string path = FileDialogWindows::ShowFileDialog(option);

                    if (std::filesystem::exists(path) && (std::filesystem::path(path).extension() == ".png" || std::filesystem::path(path).extension() == ".jpg"))
                    {
                        GlobalSettings::SkyboxTexture = TextureManager::Instance()->LoadImageTextureMap(path);
                    }
                }

                ImGui::PopStyleVar();
                ImGui::PopStyleColor(2);


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
                selectedPath = "";
                OpenGlobalSettingsPopup = false;

                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(3);

            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();

            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));
            if (ImGui::Button("Cancel", ImVec2(195, 60)))
            {
                selectedPath = "";
                OpenGlobalSettingsPopup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(3);

            ImGui::EndPopup();
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }
}

void Gui::ShowAskSceneSavePopup(std::shared_ptr<Scene> scene)
{
    if (OpenAskSceneSavePopup)
    {
        ImGui::OpenPopup("AskSceneSavePopup");

        ImVec2 nextWindowSize = ImVec2(250, 85);
        ImVec2 nextWindowPos = ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x - nextWindowSize.x) / 2, ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y - nextWindowSize.y) / 2);

        ImGui::SetNextWindowPos(nextWindowPos);
        ImGui::SetNextWindowSize(nextWindowSize);

        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
        if (ImGui::BeginPopupModal("AskSceneSavePopup", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
        {
            auto popupWindowSize = ImGui::GetWindowSize();
            
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 15));

            ImGui::Text("Do you wanna save current scene?");

            ImGui::PopStyleVar();

            //-------------------------------------------------------
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.85, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0.65, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0.45, 0, 1));
            if (ImGui::Button("OK", ImVec2(115, 20)))
            {               
                FileDialogOption option;
                option.dialogType = FileDialogType::SAVE_DIALOG;
                option.returnType = FileDialogReturnType::PICK_FILE;
                option.filters.push_back({ L"Engine Scene", L"*.json" });
                std::string path = FileDialogWindows::ShowFileDialog(option);
                if (path != "" && std::filesystem::path(path).extension() == ".json")
                    scene->Serialize(path);

                scene->DeSerialize("../Assets/NewScene.json");
                OpenAskSceneSavePopup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(3);

            ImGui::SetItemDefaultFocus();  
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));
            if (ImGui::Button("Cancel", ImVec2(115, 20)))
            {
                scene->DeSerialize("../Assets/NewScene.json");
                OpenAskSceneSavePopup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(3);

            ImGui::EndPopup();
        }

        ImGui::PopStyleColor();
    }
}

void Gui::RenderScriptGui(std::shared_ptr<Scene> scene)
{
    auto scriptPool = scene->GetRegistry()->GetComponentPool<ScriptComponent>();
    if (!scriptPool)
        return;

    for (auto entity : scriptPool->GetDenseEntitiesArray())
    {
        if (scriptPool->HasComponent(entity))
        {
            auto& scriptComponent = scriptPool->GetComponent(entity);

            for (auto& [name, script] : scriptComponent.scripts)
            {
                if (script != nullptr)
                {
                    //script->SetImGuiContext(ImGui::GetCurrentContext());
                    script->OnGui();
                }
            }
        }
    }
}