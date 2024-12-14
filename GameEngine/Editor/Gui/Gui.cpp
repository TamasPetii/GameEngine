#include "Gui.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ImGui_Glfw/imgui.h>
#include <ImGui_Glfw/imgui_impl_glfw.h>
#include <ImGui_Glfw/imgui_impl_opengl3.h>
#include <ImGui_Glfw/imgui_internal.h>
#include <tinyxml2.h>

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "FileDialogWindows.h"
#include "Gui/ViewportPanel.h"
#include "Gui/EntitiesPanel.h"
#include "Gui/ComponentPanel.h"
#include "Gui/FilesystemPanel.h"
#include "Gui/SettingsPanel.h"
#include "Gui/ConsolePanel.h"
#include "Gui/BenchmarkPanel.h"

#include "Logger/Logger.h"
#include "Manager/Managers.h"

#include "Settings/GlobalSettings.h"
#include "Render/SkyboxRenderer.h"
#include "Render/OpenGL/FramebufferGL.h"
#include "Registry/System/ScriptSystem.h"
#include "Registry/Component/ScriptComponent.h"
#include "Registry/System/Physics/PhysicsSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

bool Gui::OpenGlobalSettingsPopup = false;
bool Gui::OpenAskSceneSavePopup = false;
bool Gui::OpenNewProjectPopup = false;
bool Gui::OpenInitialPopup = false;
bool Gui::OpenInitProjectPopup = false;
bool Gui::OpenBuildProjectPopup = false;
bool Gui::OpenLoadProjectPopup = false;

bool Gui::ShowEntityWindow = true;
bool Gui::ShowSettingsWindow = true;
bool Gui::ShowBenchmarkWindow = true;
bool Gui::ShowFileSystemWindow = true;
bool Gui::ShowConsoleWindow = true;
bool Gui::ShowComponentWindow = true;
bool Gui::ShowDemoWindow = true;

std::unordered_map<std::type_index, double> Gui::m_PanelTimes;
std::unordered_map<std::type_index, double> Gui::m_AveragePanelTimes;

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
    static bool checkDeployed = true;
    static bool start = true;

    if (checkDeployed)
    {
        checkDeployed = false;
        GlobalSettings::DeployedGame = CheckIfDeployedGame(scene);
        GlobalSettings::GameViewActive = GlobalSettings::DeployedGame;
    }

    if (!GlobalSettings::DeployedGame)
    {
        if (start)
        {
            start = false;
            GlobalSettings::Inited = CheckAndLoadInitData();
        }

        //If installed path and MsBuild is not set yet
        if (!GlobalSettings::Inited)
            OpenInitialPopup = true;

        //If project path is not set yet
        if (!OpenInitialPopup && !OpenNewProjectPopup && !OpenLoadProjectPopup && GlobalSettings::ProjectPath == "")
            OpenInitProjectPopup = true;
    }

    PreRender();
    RenderDockSpace(scene);

    if (!(OpenInitialPopup || OpenLoadProjectPopup || OpenInitProjectPopup || OpenNewProjectPopup || OpenBuildProjectPopup))
    {
        if (!GlobalSettings::GameViewActive)
        {
            CreateProjectSceneImage(deltaTime);
        }

        if(!GlobalSettings::GameViewActive && ShowDemoWindow)
            ImGui::ShowDemoWindow();

        if (ShowFileSystemWindow)
        { //FilesystemPanel
            auto start = std::chrono::high_resolution_clock::now();
            FilesystemPanel::Render();
            auto end = std::chrono::high_resolution_clock::now();
            m_PanelTimes[Unique::typeIndex<FilesystemPanel>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        }

        if (ShowConsoleWindow)
        { //ConsolePanel
            auto start = std::chrono::high_resolution_clock::now();
            ConsolePanel::Render();
            auto end = std::chrono::high_resolution_clock::now();
            m_PanelTimes[Unique::typeIndex<ConsolePanel>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        }

        if (ShowEntityWindow)
        { //EntitiesPanel
            auto start = std::chrono::high_resolution_clock::now();
            EntitiesPanel::Render(scene);
            auto end = std::chrono::high_resolution_clock::now();
            m_PanelTimes[Unique::typeIndex<EntitiesPanel>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        }

        if (ShowComponentWindow)
        { //ComponentPanel
            auto start = std::chrono::high_resolution_clock::now();
            ComponentPanel::Render(scene);
            auto end = std::chrono::high_resolution_clock::now();
            m_PanelTimes[Unique::typeIndex<ComponentPanel>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        }

        if (ShowSettingsWindow)
        { //SettingsPanel
            auto start = std::chrono::high_resolution_clock::now();
            SettingsPanel::Render(scene);
            auto end = std::chrono::high_resolution_clock::now();
            m_PanelTimes[Unique::typeIndex<SettingsPanel>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        }

        { //ViewportPanel
            auto start = std::chrono::high_resolution_clock::now();
            ViewportPanel::Render(scene, deltaTime);
            auto end = std::chrono::high_resolution_clock::now();
            m_PanelTimes[Unique::typeIndex<ViewportPanel>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        }

        static float time = 0;
        static int counter = 0;

        time += deltaTime;
        counter += 1;

        if (time > 1)
        {
            for (auto& timeData : m_PanelTimes)
            {
                m_AveragePanelTimes[timeData.first] = (timeData.second / counter) / 1000.0;
                timeData.second = 0;
            }

            time = 0;
            counter = 0;
        }

        if(ShowBenchmarkWindow)
            BenchmarkPanel::Render(scene, m_AveragePanelTimes);
    }

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
        RenderScriptGui(scene);

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
                    OpenNewProjectPopup = true;
                }

                if (ImGui::MenuItem("Load Project"))
                {
                    OpenLoadProjectPopup = true;
                }

                if (ImGui::MenuItem("Build Project"))
                {
                    OpenBuildProjectPopup = true;
                }

                ImGui::Separator();

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

                ImGui::Separator();

                if (ImGui::MenuItem("Settings"))
                {
                    OpenGlobalSettingsPopup = true;
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Windows"))
            {
                if (ImGui::MenuItem("Entity Window", nullptr, &ShowEntityWindow))
                {
                }

                if (ImGui::MenuItem("Component Window", nullptr, &ShowComponentWindow))
                {
                }

                if (ImGui::MenuItem("Settings Window", nullptr, &ShowSettingsWindow))
                {
                }

                if (ImGui::MenuItem("Benchmark Window", nullptr, &ShowBenchmarkWindow))
                {
                }

                if (ImGui::MenuItem("FileSystem Window", nullptr, &ShowFileSystemWindow))
                {
                }

                if (ImGui::MenuItem("Console Window", nullptr, &ShowConsoleWindow))
                {
                }

                if (ImGui::MenuItem("Demo Window", nullptr, &ShowDemoWindow))
                {
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
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
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
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.35f);

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
    Gui::ShowInitialPopup();
    ComponentPanel::ShowAssetPopup(scene->GetRegistry());
    Gui::ShowGlobalSettingsPopup();
    Gui::ShowAskSceneSavePopup(scene);
    Gui::ShowNewProjectPopup(scene);
    Gui::ShowLoadProjectPopup(scene);
    Gui::ShowInitProjectPopup();
    Gui::BuildProjectToDeployedGame(scene);
}

void Gui::ShowGlobalSettingsPopup()
{
    static std::string selectedPath = "";
    static auto noTexture = TextureManager::Instance()->LoadImageTexture("../Assets/Icons/NoTexture.png");

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

                ImGui::Text("Gravity");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                ImGui::DragFloat3("##Gravity##GlobalSettingsPopup", &PhysicsSystem::gravity.x);

                ImGui::SeparatorText("Skybox");

                ImGui::Text("Use Skybox");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                ImGui::Checkbox("##UseSkybox##GlobalSettingsPopup", &SkyboxRenderer::UseSkybox);

                ImGui::Text("Rotation Speed");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                ImGui::DragFloat3("##Rotation Speed##GlobalSettingsPopup", &SkyboxRenderer::SkyboxRotationSpeed.x, 0.02f);

                ImGui::Text("Rotation Dir");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                ImGui::SliderInt3("##Rotation Dir##GlobalSettingsPopup", &SkyboxRenderer::SkyboxRotationDirection.x, -1, 1);

                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                if(ImGui::Button("Reset Rotation##GlobalSettingsPopup", ImVec2(ImGui::GetContentRegionAvail().x - 15, 18)))
                {
                    SkyboxRenderer::SkyboxRotation = glm::vec3(0, 0, 0);
                }

                ImGui::Text("Skybox Texture");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                auto skyboxTexture = SkyboxRenderer::SkyboxTexture ? TextureManager::Instance()->LoadImageTexture(SkyboxRenderer::SkyboxTexture->GetPath()) : noTexture;

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
                        SkyboxRenderer::SkyboxTexture = TextureManager::Instance()->LoadImageTextureMap(path);
                    }
                }

                ImGui::PopStyleVar();
                ImGui::PopStyleColor(2);

                ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));

                if (ImGui::Button("X##NormalMaterialTexture", ImVec2(skyboxTextureWidth, 18)))
                    SkyboxRenderer::SkyboxTexture = nullptr;

                ImGui::PopStyleColor(3);
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

                scene->DeSerialize(GlobalSettings::ProjectPath + "/Assets/Scene/NewScene.json");
                ViewportPanel::m_ViewportSizeChanged = true;
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
                scene->DeSerialize(GlobalSettings::ProjectPath + "/Assets/Scene/NewScene.json");
                ViewportPanel::m_ViewportSizeChanged = true;
                OpenAskSceneSavePopup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(3);

            ImGui::EndPopup();
        }

        ImGui::PopStyleColor();
    }
}

void Gui::ShowNewProjectPopup(std::shared_ptr<Scene> scene)
{
    static std::string newProjectName = "";
    static std::string newProjectPath = "";

    if (OpenNewProjectPopup)
    {
        ImGui::OpenPopup("NewProjectPopup");

        ImVec2 nextWindowSize = ImVec2(350, 175);
        ImVec2 nextWindowPos = ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x - nextWindowSize.x) / 2, ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y - nextWindowSize.y) / 2);

        ImGui::SetNextWindowPos(nextWindowPos);
        ImGui::SetNextWindowSize(nextWindowSize);

        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
        if (ImGui::BeginPopupModal("NewProjectPopup", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
        {
            auto popupWindowSize = ImGui::GetWindowSize();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 15));

            std::vector<char> projectNameVec(100);
            std::copy(newProjectName.begin(), newProjectName.end(), projectNameVec.begin());

            ImGui::Text("Project Name");
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::InputText("##Project Name Input Text", projectNameVec.data(), projectNameVec.size()))
            {
                newProjectName = std::string(projectNameVec.data());
            }

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2, 0.2, 0.85, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2, 0.2, 0.65, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2, 0.2, 0.45, 1));
            ImGui::Text("Project Path");
            if (ImGui::Button(newProjectPath.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 18)))
            {
                FileDialogOption option;
                option.dialogType = FileDialogType::OPEN_DIALOG;
                option.returnType = FileDialogReturnType::PICK_FOLDER;
                std::string path = FileDialogWindows::ShowFileDialog(option);
                if (path != "")
                    newProjectPath = path;
            }
            ImGui::PopStyleColor(3);

            ImGui::PopStyleVar();

            //-------------------------------------------------------
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.85, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0.65, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0.45, 0, 1));
            if (ImGui::Button("OK", ImVec2(115, 20)))
            {
                bool success = true;
                
                if (newProjectName != "" && newProjectPath != "")
                    success = success && GenerateProject(newProjectPath, newProjectName);
                else
                    success = false;
                
                if (success)
                {
                    std::string appdataPath{ std::getenv("APPDATA") };
                    std::replace(appdataPath.begin(), appdataPath.end(), '\\', '/');

                    std::string gameEngineAppdataFolderPath = appdataPath + "/GameEngine";
                    std::string projectsJsonFilePath = appdataPath + "/GameEngine/Projects.json";

                    if (std::filesystem::exists(gameEngineAppdataFolderPath))
                    {
                        nlohmann::json data;

                        if (std::filesystem::exists(projectsJsonFilePath))
                        {
                            std::ifstream projectsJsonFile(projectsJsonFilePath);
                            data = nlohmann::json::parse(projectsJsonFile);
                            projectsJsonFile.close();
                        }
                        else
                        {
                            data["Projects"] = nlohmann::json::array();
                        }

                        std::string projPath = newProjectPath + "/" + newProjectName;

                        bool isProjectRegistered = false;
                        for (auto& projectData : data["Projects"])
                        {
                            if (static_cast<std::string>(projectData["Path"]) == projPath)
                                isProjectRegistered = true;
                        }

                        if (!isProjectRegistered)
                        {
                            nlohmann::json projectData;
                            projectData["Path"] = projPath;
                            data["Projects"].push_back(projectData);

                            std::ofstream output(projectsJsonFilePath);
                            if (output.is_open())
                                output << data.dump(4);
                            output.close();
                        }
                    }

                    GlobalSettings::ProjectPath = newProjectPath + "/" + newProjectName;
                    FilesystemPanel::fileSystemPath = std::filesystem::path(GlobalSettings::ProjectPath);
                    scene->DeSerialize(GlobalSettings::ProjectPath + "/Assets/Scene/NewScene.json");
                    
                    LOG_INFO("NewProjectPopup", "Succesfully created and loaded project: " + GlobalSettings::ProjectPath);

                    OpenNewProjectPopup = false;
                    newProjectName = "";
                    newProjectPath = "";
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::PopStyleColor(3);

            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            ImGui::SetCursorPosX(225);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));
            if (ImGui::Button("Cancel", ImVec2(115, 20)))
            {
                if (GlobalSettings::ProjectPath == "")
                    OpenInitProjectPopup = true;

                OpenNewProjectPopup = false;
                newProjectName = "";
                newProjectPath = "";
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(3);

            ImGui::EndPopup();
        }

        ImGui::PopStyleColor();
    }
}

void Gui::ShowInitialPopup()
{
    static std::string compilerPath = "";
    static std::string defaultPath = "";

    if (OpenInitialPopup)
    {
        ImGui::OpenPopup("InitialPopup");

        ImVec2 nextWindowSize = ImVec2(350, 175);
        ImVec2 nextWindowPos = ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x - nextWindowSize.x) / 2, ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y - nextWindowSize.y) / 2);

        ImGui::SetNextWindowPos(nextWindowPos);
        ImGui::SetNextWindowSize(nextWindowSize);

        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
        if (ImGui::BeginPopupModal("InitialPopup", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
        {
            auto popupWindowSize = ImGui::GetWindowSize();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 15));

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2, 0.2, 0.85, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2, 0.2, 0.65, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2, 0.2, 0.45, 1));

            ImGui::Text("Default Path");
            if (ImGui::Button(defaultPath.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 18)))
            {
                FileDialogOption option;
                option.dialogType = FileDialogType::OPEN_DIALOG;
                option.returnType = FileDialogReturnType::PICK_FOLDER;
                std::string path = FileDialogWindows::ShowFileDialog(option);
                if (std::filesystem::exists(path) && std::filesystem::exists(path + "/Engine.json"))
                    defaultPath = path;
            }

            ImGui::Text("Compiler Path (MSBuild.exe)");
            if (ImGui::Button(compilerPath.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 18)))
            {
                FileDialogOption option;
                option.dialogType = FileDialogType::OPEN_DIALOG;
                option.returnType = FileDialogReturnType::PICK_FILE;
                std::string path = FileDialogWindows::ShowFileDialog(option);
                if (std::filesystem::exists(path) && std::filesystem::path(path).filename() == "MSBuild.exe")
                    compilerPath = path;
            }
            ImGui::PopStyleColor(3);

            ImGui::PopStyleVar();

            //-------------------------------------------------------
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.85, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0.65, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0.45, 0, 1));
            ImGui::SetItemDefaultFocus();
            if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvail().x, 20)))
            {
                if (defaultPath != "" && compilerPath != "")
                {
                    GenerateInitData(defaultPath, compilerPath);
                    GlobalSettings::CompilerPath = compilerPath;
                    GlobalSettings::DefaultEnginePath = defaultPath;
                    UpdateScriptVcxprojPaths();
                    defaultPath = "";
                    compilerPath = "";
                    OpenInitialPopup = false;
                    GlobalSettings::Inited = true;
                    ImGui::CloseCurrentPopup();
                }
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

    if (GlobalSettings::GameViewActive && ScriptSystem::SetImGuiContextFunction)
    {
        ScriptSystem::SetImGuiContextFunction(ImGui::GetCurrentContext());

        for (auto entity : scriptPool->GetDenseEntitiesArray())
        {
            if (scriptPool->HasComponent(entity))
            {
                auto& scriptComponent = scriptPool->GetComponent(entity);

                for (auto& [name, script] : scriptComponent.scripts)
                {
                    if (script != nullptr)
                    {
                        script->OnGui();
                    }
                }
            }
        }
    }
}

bool Gui::GenerateProject(const std::string& parentPath, const std::string& name)
{
    std::string projectFolderPath = parentPath + "/" + name;
    std::string projectManifestFilePath = projectFolderPath + "/" + "Project.json";

    //Project is already generated
    if (std::filesystem::exists(projectFolderPath) && std::filesystem::exists(projectManifestFilePath))
        return false;

    //Generate Project Folder
    std::filesystem::create_directories(projectFolderPath);

    //Generate Project Manifest File
    nlohmann::json manifestFile;
    manifestFile["name"] = name;
    manifestFile["version"] = "1.0";
    manifestFile["date"] = Logger::Instance()->GetCurrentTimestamp();

    std::ofstream output(projectManifestFilePath);
    if (output.is_open())
        output << manifestFile.dump(4);
    output.close();

    bool copySuccess = true;
    copySuccess = copySuccess && CopyFolderAndContent(GlobalSettings::DefaultEnginePath, "Scripts", projectFolderPath);
    copySuccess = copySuccess && CopyFolderAndContent(GlobalSettings::DefaultEnginePath, "Assets", projectFolderPath);

    return copySuccess;
}

bool Gui::CopyFolderAndContent(const std::string& sourceFolderPath, const std::string& folderPath, const std::string projectPath)
{
    std::string defaultFolderPath = sourceFolderPath + "/" + folderPath;

    if (!std::filesystem::exists(defaultFolderPath)) {
        std::cerr << "Default script folder does not exist!.\n";
        return false;
    }

    if (!std::filesystem::is_directory(projectPath))
    {
        std::cerr << "New Project folder does not exist!.\n";
        return false;
    }

    std::filesystem::create_directories(projectPath + "/" + folderPath);

    const auto& defaultEngineFsPath = std::filesystem::path(sourceFolderPath);
    for (const auto& entry : std::filesystem::recursive_directory_iterator(defaultFolderPath)) {
        const auto& entryFsPath = entry.path();

        //Get the path starting from the Default Engine Folder -> Script/Script/...
        std::filesystem::path relativePath = std::filesystem::relative(entryFsPath, defaultEngineFsPath);

        //Convert \\ characters to / for concetanation
        std::string relativePathStr = relativePath.string();
        std::replace(relativePathStr.begin(), relativePathStr.end(), '\\', '/');

        //Path where entry needs to be copied
        std::filesystem::path targetPath = std::filesystem::path(projectPath + "/" + relativePathStr);

        if (std::filesystem::is_directory(entryFsPath))
            std::filesystem::create_directories(targetPath);
        else if (std::filesystem::is_regular_file(entryFsPath))
            std::filesystem::copy(entryFsPath, targetPath, std::filesystem::copy_options::overwrite_existing);
    }
}

void Gui::GenerateInitData(const std::string& defaultPath, const std::string& compilerPath)
{
    std::string appdataPath{ std::getenv("APPDATA") };
    std::replace(appdataPath.begin(), appdataPath.end(), '\\', '/');

    std::string engineFolder = appdataPath + "/GameEngine";
    if (!std::filesystem::exists(engineFolder))
        std::filesystem::create_directories(std::filesystem::path(engineFolder));

    std::string initFilePath = engineFolder + "/Init.json";

    nlohmann::json initFile;
    initFile["compilerPath"] = compilerPath;
    initFile["defaultPath"] = defaultPath;

    std::ofstream output(initFilePath);
    if (output.is_open())
        output << initFile.dump(4);
    output.close();
}

bool Gui::CheckAndLoadInitData()
{
    std::string appdataPath{std::getenv("APPDATA")};
    std::replace(appdataPath.begin(), appdataPath.end(), '\\', '/');

    std::string engineFolder = appdataPath + "/GameEngine";
    if (!std::filesystem::exists(engineFolder))
        return false;

    std::string initFilePath = engineFolder + "/Init.json";
    if (!std::filesystem::exists(initFilePath))
        return false;

    std::ifstream input(initFilePath);
    nlohmann::json data = nlohmann::json::parse(input);

    if (data.find("compilerPath") == data.end() || data.find("defaultPath") == data.end())
        return false;
            
    if (!std::filesystem::exists(data["defaultPath"]) || !std::filesystem::exists(data["compilerPath"]))
        return false;

    GlobalSettings::CompilerPath = data["compilerPath"];
    GlobalSettings::DefaultEnginePath = data["defaultPath"];

    return true;
}

void Gui::ShowInitProjectPopup()
{
    if (OpenInitProjectPopup)
    {
        ImGui::OpenPopup("InitProjectPopup");

        ImVec2 nextWindowSize = ImVec2(350, 200);
        ImVec2 nextWindowPos = ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x - nextWindowSize.x) / 2, ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y - nextWindowSize.y) / 2);

        ImGui::SetNextWindowPos(nextWindowPos);
        ImGui::SetNextWindowSize(nextWindowSize);

        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
        if (ImGui::BeginPopupModal("InitProjectPopup", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
        {
            auto popupWindowSize = ImGui::GetWindowSize();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 15));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2, 0.2, 0.85, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2, 0.2, 0.65, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2, 0.2, 0.45, 1));

            if (ImGui::Button("New Project##InitProjectPopup", ImVec2(ImGui::GetContentRegionAvail().x, 75)))
            {
                OpenNewProjectPopup = true;
                OpenInitProjectPopup = false;
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::Button("Load Project##InitProjectPopup", ImVec2(ImGui::GetContentRegionAvail().x, 75)))
            {
                OpenLoadProjectPopup = true;
                OpenInitProjectPopup = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar();

            ImGui::EndPopup();
        }

        ImGui::PopStyleColor();
    }
}

bool Gui::CheckIfDeployedGame(std::shared_ptr<Scene> scene)
{
    char path[MAX_PATH];
    DWORD result = GetModuleFileNameA(NULL, path, MAX_PATH);

    std::string exePath(path);
    auto exeFsPath = std::filesystem::path(exePath);
    std::string exeParentFsPathStr = exeFsPath.parent_path().string();
    std::replace(exeParentFsPathStr.begin(), exeParentFsPathStr.end(), '\\', '/');

    std::string buildDir = exeFsPath.parent_path().parent_path().string();
    std::replace(buildDir.begin(), buildDir.end(), '\\', '/');

    std::string buildJsPath = exeParentFsPathStr + "/Build.json";
    std::string sceneJsPath = exeParentFsPathStr + "/StartScene.json";

    if (std::filesystem::exists(buildJsPath) && std::filesystem::exists(sceneJsPath))
    {
        GlobalSettings::ProjectPath = buildDir;
        scene->DeSerialize(sceneJsPath);
        scene->StartGame();
        return true;
    }

    return false;
}

void Gui::BuildProjectToDeployedGame(std::shared_ptr<Scene> scene)
{
    static std::string newProjectName = "";
    static std::string newProjectPath = "";

    if (OpenBuildProjectPopup)
    {
        ImGui::OpenPopup("BuildProjectPopup");

        ImVec2 nextWindowSize = ImVec2(350, 175);
        ImVec2 nextWindowPos = ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x - nextWindowSize.x) / 2, ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y - nextWindowSize.y) / 2);

        ImGui::SetNextWindowPos(nextWindowPos);
        ImGui::SetNextWindowSize(nextWindowSize);

        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
        if (ImGui::BeginPopupModal("BuildProjectPopup", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
        {
            auto popupWindowSize = ImGui::GetWindowSize();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 15));

            std::vector<char> projectNameVec(100);
            std::copy(newProjectName.begin(), newProjectName.end(), projectNameVec.begin());

            ImGui::Text("Game Folder Name");
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::InputText("##Project Name Input Text", projectNameVec.data(), projectNameVec.size()))
            {
                newProjectName = std::string(projectNameVec.data());
            }

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2, 0.2, 0.85, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2, 0.2, 0.65, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2, 0.2, 0.45, 1));
            ImGui::Text("Deployed Game Parent Path");
            if (ImGui::Button(newProjectPath.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 18)))
            {
                FileDialogOption option;
                option.dialogType = FileDialogType::OPEN_DIALOG;
                option.returnType = FileDialogReturnType::PICK_FOLDER;
                std::string path = FileDialogWindows::ShowFileDialog(option);
                if (path != "")
                    newProjectPath = path;
            }
            ImGui::PopStyleColor(3);

            ImGui::PopStyleVar();

            //-------------------------------------------------------
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.85, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0.65, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0.45, 0, 1));
            if (ImGui::Button("OK", ImVec2(115, 20)))
            {
                bool success = true;

                if (newProjectName != "" && newProjectPath != "")
                    success = success && GenerateBuildGameProject(scene, newProjectPath, newProjectName);
                else
                    success = false;

                if (success)
                {
                    LOG_ERROR("BuildProjectPopup", "Successfully build game project: " + newProjectPath + "/" + newProjectName);

                    OpenBuildProjectPopup = false;
                    newProjectName = "";
                    newProjectPath = "";
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::PopStyleColor(3);

            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            ImGui::SetCursorPosX(225);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));
            if (ImGui::Button("Cancel", ImVec2(115, 20)))
            {
                OpenBuildProjectPopup = false;
                newProjectName = "";
                newProjectPath = "";
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(3);

            ImGui::EndPopup();
        }

        ImGui::PopStyleColor();
    }
}

bool Gui::GenerateBuildGameProject(std::shared_ptr<Scene> scene, const std::string& parentPath, const std::string& name)
{
    std::string projectFolderPath = parentPath + "/" + name;

    //Project is already generated
    if (std::filesystem::exists(projectFolderPath))
        return false;

    //Generate Project Folder
    std::filesystem::create_directories(projectFolderPath);

    bool copySuccess = true;
    copySuccess = copySuccess && CopyFolderAndContent(GlobalSettings::ProjectPath, "Scripts/x64", projectFolderPath);
    copySuccess = copySuccess && CopyFolderAndContent(GlobalSettings::ProjectPath, "Assets", projectFolderPath);
    copySuccess = copySuccess && CopyFolderAndContent(GlobalSettings::DefaultEnginePath, "Build", projectFolderPath);
    copySuccess = copySuccess && CopyFolderAndContent(GlobalSettings::DefaultEnginePath, "Engine/Render/Shader", projectFolderPath);

    //Generate Project Manifest File
    nlohmann::json manifestFile;
    manifestFile["name"] = name;
    manifestFile["version"] = "1.0";
    manifestFile["date"] = Logger::Instance()->GetCurrentTimestamp();
    scene->Serialize(projectFolderPath + "/Build/StartScene.json");

    std::ofstream output(projectFolderPath + "/Build/Build.json");
    if (output.is_open())
        output << manifestFile.dump(4);
    output.close();

    return copySuccess;
}

bool Gui::UpdateScriptVcxprojPaths()
{
    std::string defaultPath = GlobalSettings::DefaultEnginePath;
    std::string scriptVcxproj = defaultPath + "/Scripts/Scripts/Scripts.vcxproj";
    tinyxml2::XMLDocument doc;

    // Load the .vcxproj file
    if (doc.LoadFile(scriptVcxproj.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load XML file!" << std::endl;
        return false;
    }

    // Get the root element
    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        std::cerr << "Invalid XML structure: no root element." << std::endl;
        return false;
    }

    for (tinyxml2::XMLElement* propertyGroup = root->FirstChildElement("PropertyGroup"); propertyGroup != nullptr; propertyGroup = propertyGroup->NextSiblingElement("PropertyGroup")) {
        const char* condition = propertyGroup->Attribute("Condition");
        if (condition) {
            // Look for the IncludePath and LibraryPath elements
            tinyxml2::XMLElement* includePathElem = propertyGroup->FirstChildElement("IncludePath");
            tinyxml2::XMLElement* libraryPathElem = propertyGroup->FirstChildElement("LibraryPath");

            std::string conditionStr = std::string(condition);
            if (conditionStr == "'$(Configuration)|$(Platform)'=='Debug|Win32'" ||
                conditionStr == "'$(Configuration)|$(Platform)'=='Debug|x64'" ||
                conditionStr == "'$(Configuration)|$(Platform)'=='Release|Win32'" ||
                conditionStr == "'$(Configuration)|$(Platform)'=='Release|x64'")
            {
                if (includePathElem) {
                    std::string include = defaultPath + "/External/include;";
                    std::string includePhysx = defaultPath + "/External/include/physx;";
                    std::string includeEngine = defaultPath + "/Engine;";
                    std::string includePath = include + includePhysx + includeEngine + "$(IncludePath)";
                    includePathElem->SetText(includePath.c_str());
                }
                if (libraryPathElem) {
                    std::string lib = defaultPath + "/External/lib;";
                    std::string includeEngine = defaultPath + "/Build;";
                    std::string libraryPath = lib + includeEngine + "$(LibraryPath)";
                    libraryPathElem->SetText(libraryPath.c_str());
                }
            }
        }
    }

    if (doc.SaveFile(scriptVcxproj.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to save the updated file." << std::endl;
        return false;
    }

    return true;
}

void Gui::ShowLoadProjectPopup(std::shared_ptr<Scene> scene)
{
    static bool refreshImages = true;
    static std::string selectedPath = "";

    if (OpenLoadProjectPopup)
    {
        ImGui::OpenPopup("LoadProjectPopup");

        ImVec2 nextWindowSize = ImVec2(600, 600);
        ImVec2 nextWindowPos = ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x - nextWindowSize.x) / 2, ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y - nextWindowSize.y) / 2);

        ImGui::SetNextWindowPos(nextWindowPos);
        ImGui::SetNextWindowSize(nextWindowSize);

        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.1f, 0.1f, 0.1f, 1.f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        if (ImGui::BeginPopupModal("LoadProjectPopup", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
        {
            auto popupWindowSize = ImGui::GetWindowSize();
            auto textureManager = TextureManager::Instance();

            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.22f, 0.22f, 0.22f, 1.f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
            if (ImGui::BeginChild("Cards##LoadProjectPopup", ImVec2(ImVec2(popupWindowSize.x, popupWindowSize.y * 0.85)), true))
            {
                std::string appdataPath{ std::getenv("APPDATA") };
                std::replace(appdataPath.begin(), appdataPath.end(), '\\', '/');

                std::string gameEngineAppdataFolderPath = appdataPath + "/GameEngine";
                std::string projectsJsonFilePath = appdataPath + "/GameEngine/Projects.json";
                
                if (std::filesystem::exists(projectsJsonFilePath))
                {
                    std::ifstream input(projectsJsonFilePath);
                    nlohmann::json data = nlohmann::json::parse(input);

                    int counter = 0;

                    for (auto& projectData : data["Projects"])
                    {
                        if (!(std::filesystem::exists(static_cast<std::string>(projectData["Path"])) && std::filesystem::exists(static_cast<std::string>(projectData["Path"]) + "/Project.json")))
                            continue;

                        std::string projectPath = static_cast<std::string>(projectData["Path"]);

                        if (selectedPath == projectPath)
                            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.66f, 0.66f, 0.12f, 1));
                        else
                            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12f, 0.12f, 0.12f, 1));

                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 0));
                        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 2);

                        bool is_focused = false;
                        std::string name = std::filesystem::path(projectPath).filename().string();
                        if (ImGui::BeginChild(projectPath.c_str(), ImVec2(140, 180), true))
                        {
                            is_focused = ImGui::IsWindowFocused();

                            std::string texturePath = gameEngineAppdataFolderPath + "/" + name +".png";

                            unsigned int textureID = 0;
                            if (std::filesystem::exists(texturePath))
                            {
                                if (refreshImages && textureManager->RefAllTextues().find(texturePath) != textureManager->RefAllTextues().end())
                                    textureManager->RefAllTextues()[texturePath] = nullptr;

                                textureID = TextureManager::Instance()->LoadImageTexture(texturePath)->GetTextureID();
                            }

                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                            ImGui::Image((ImTextureID)textureID, ImVec2(140, 140), ImVec2(0, 1), ImVec2(1, 0));
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
                            selectedPath = projectPath;
                        }

                        counter++;
                    }

                    refreshImages = false;
                    input.close();
                }
            }

            ImGui::EndChild();
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();

            //-------------------------------------------------------

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0.85, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0.65, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0.45, 0, 1));
            if (ImGui::Button("OK##LoadProjectPopup", ImVec2(195, 60)))
            {
                if (selectedPath != "" && std::filesystem::exists(selectedPath) && std::filesystem::exists(selectedPath + "/Project.json"))
                {
                    GlobalSettings::ProjectPath = selectedPath;
                    FilesystemPanel::fileSystemPath = std::filesystem::path(selectedPath);
                    ViewportPanel::m_ViewportSizeChanged = true;
                    scene->DeSerialize(GlobalSettings::ProjectPath + "/Assets/Scene/NewScene.json");

                    LOG_INFO("LoadProjectPopup", "Succesfully loaded project: " + selectedPath);

                    refreshImages = true;
                    selectedPath = "";
                    OpenLoadProjectPopup = false;
                    ImGui::CloseCurrentPopup();
                }
                else
                    LOG_ERROR("LoadProjectPopup", "Project does not exist! Path = " + selectedPath);
            }
            ImGui::PopStyleColor(3);

            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0.85, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0.65, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0.45, 1));
            if (ImGui::Button("Load##ProjectLoadPopup", ImVec2(195, 60)))
            {
                FileDialogOption option;
                option.dialogType = FileDialogType::OPEN_DIALOG;
                option.returnType = FileDialogReturnType::PICK_FOLDER;
                std::string path = FileDialogWindows::ShowFileDialog(option);

                if (path != "" && std::filesystem::exists(path) && std::filesystem::exists(path + "/Project.json"))
                {
                    std::string appdataPath{ std::getenv("APPDATA") };
                    std::replace(appdataPath.begin(), appdataPath.end(), '\\', '/');

                    std::string gameEngineAppdataFolderPath = appdataPath + "/GameEngine";
                    std::string projectsJsonFilePath = appdataPath + "/GameEngine/Projects.json";

                    if (std::filesystem::exists(gameEngineAppdataFolderPath))
                    {
                        nlohmann::json data;

                        if (std::filesystem::exists(projectsJsonFilePath))
                        {
                            std::ifstream projectsJsonFile(projectsJsonFilePath);
                            data = nlohmann::json::parse(projectsJsonFile);
                            projectsJsonFile.close();
                        }
                        else
                        {
                            data["Projects"] = nlohmann::json::array();
                        }

                        bool isProjectRegistered = false;
                        for (auto& projectData : data["Projects"])
                        {
                            if (static_cast<std::string>(projectData["Path"]) == path)
                                isProjectRegistered = true;
                        }

                        if (!isProjectRegistered)
                        {
                            nlohmann::json projectData;
                            projectData["Path"] = path;
                            data["Projects"].push_back(projectData);

                            std::ofstream output(projectsJsonFilePath);
                            if (output.is_open())
                                output << data.dump(4);
                            output.close();
                        }
                    }

                    selectedPath = path;
                }
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.85, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65, 0, 0, 1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45, 0, 0, 1));
            if (ImGui::Button("Cancel", ImVec2(195, 60)))
            {
                if (GlobalSettings::ProjectPath == "")
                    OpenInitProjectPopup = true;

                refreshImages = true;
                selectedPath = "";
                OpenLoadProjectPopup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopStyleColor(3);

            ImGui::EndPopup();
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }
}

void Gui::CreateProjectSceneImage(float deltaTime)
{
    static float time = 0;
    time += deltaTime;

    if (time > 60)
    {
        time = 0;

        std::string appdataPath{ std::getenv("APPDATA") };
        std::replace(appdataPath.begin(), appdataPath.end(), '\\', '/');
        std::string gameEngineAppdataFolderPath = appdataPath + "/GameEngine";

        if (std::filesystem::exists(gameEngineAppdataFolderPath))
        {
            auto resourceManager = ResourceManager::Instance();
            auto fbo = resourceManager->GetFbo("Main");
            auto texture = fbo->GetTextureID("main");
            auto& textureSpec = fbo->GetTextureSpec("main");

            fbo->Bind();
            glReadBuffer(textureSpec.attachment);

            int width = fbo->GetSize().x;
            int height = fbo->GetSize().y;
            std::vector<float> pixels(width * height * 4);
            std::vector<unsigned char> pixelsNormalized(width * height * 4);

            glReadPixels(0, 0, width, height, textureSpec.format, textureSpec.type, pixels.data());

            for(int i = 0; i < pixelsNormalized.size(); ++i)
                pixelsNormalized[i] = static_cast<unsigned char>(glm::clamp(pixels[i] * 255.0f, 0.0f, 255.0f));

            int resizedWidth = 300;
            int resizedHeight = 300;
            std::vector<unsigned char> resizedPixels(resizedWidth * resizedHeight * 4);
            stbir_resize_uint8(pixelsNormalized.data(), width, height, 0, resizedPixels.data(), resizedWidth, resizedHeight, 0, 4);

            stbi_flip_vertically_on_write(1);
            stbi_write_png((gameEngineAppdataFolderPath + "/" + std::filesystem::path(GlobalSettings::ProjectPath).filename().string() + ".png").c_str(), resizedWidth, resizedHeight, 4, resizedPixels.data(), resizedWidth * 4);
            fbo->UnBind();
        }
    }
}