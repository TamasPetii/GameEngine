#include "Interface.h"
#include "../../../GameScript/Script.h"

std::string Interface::label;

Renderer* Interface::mRenderer;

Interface::Interface(GLFWwindow* window, Renderer* renderer)
{
    mWindow = window;
    mRenderer = renderer;

    pauseButtonImage = ImageTexture::LoadTexture("Assets/Images/Interface/ViewPort/Pause.png");
    playButtonImage = ImageTexture::LoadTexture("Assets/Images/Interface/ViewPort/Play.png");
    stopButtonImage = ImageTexture::LoadTexture("Assets/Images/Interface/ViewPort/Stop.png");
    fileImage = ImageTexture::LoadTexture("Assets/Images/Interface/FileSystem/File.png");
    folderImage = ImageTexture::LoadTexture("Assets/Images/Interface/FileSystem/Directory.png");
    parentImage = ImageTexture::LoadTexture("Assets/Images/Interface/FileSystem/Parent.png");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.33f, 0.3f, 0.3f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
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
    colors[ImGuiCol_Button] = ImVec4(0.35f, 0.35f, 0.35f, 0.54f);
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


void Interface::Update()
{
    mRenderer->Get_Camera()->Update();

    if (mViewPortResize)
    {
        mViewPortResize = false;
        mRenderer->Get_Camera()->SetProjMatrix(mViewPortSize.x, mViewPortSize.y);
        mRenderer->Get_FrameBufferObject("scene")->Resize(mViewPortSize.x, mViewPortSize.y);
    }
}

void Interface::PreRender()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void Interface::PostRender()
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

void Interface::Render()
{
    Interface::PreRender();
    Interface::RenderDockSpace();
    Interface::RenderViewPortWindow();
    Interface::RenderComponentsWindow();
    Interface::RenderSettingsWindow();
    Interface::EntityWindow();
    Interface::FileSystemWindow();
    ImGui::ShowDemoWindow();
    Interface::PostRender();
}

void Interface::RenderDockSpace()
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
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

    ImGui::Begin("DockSpace Demo", nullptr, window_flags);

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

    //Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load"))
            {
                std::ifstream in("Assets/Scenes/scene.json");
                json data = json::parse(in);
                in.close();
                mRenderer->Ref_Scene() = new Scene(data["Scene"]);
            }
            if (ImGui::MenuItem("Save"))
            {
                std::ofstream out("Assets/Scenes/scene.json");
                out << mRenderer->Get_Scene()->SaveToJson().dump(4);
                out.close();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (scriptEntity != nullptr)
    {
        std::cout << "Create Script Popup" << std::endl;
        ImGui::OpenPopup("Create Script");
    }

    if (ImGui::BeginPopupModal("Create Script", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static char str0[128] = "";
        ImGui::InputText("input text", str0, IM_ARRAYSIZE(str0));

        if (ImGui::Button("OK", ImVec2(120, 0))) 
        { 
            if (str0 != "")
            {
                if (!scriptEntity->HasComponent<ScriptComponent>())
                    scriptEntity->AddComponent(new ScriptComponent);

                ScriptComponent::GENERATE_SCRIPT(std::string(str0));

                auto script = scriptEntity->GetComponent<ScriptComponent>()->AttachScript(std::string(str0));

                if(script)
                    script->AttachEntity(scriptEntity);

                scriptEntity = nullptr;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120, 0))) 
        { 
            ImGui::CloseCurrentPopup();
            scriptEntity = nullptr;
        }
        ImGui::EndPopup();
    }


    ImGui::End();
}

void Interface::RenderViewPortWindow()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
    ImGui::Begin("ViewPort", nullptr, ImGuiWindowFlags_NoCollapse);

    Camera_KeyboardEvent();
    Camera_MouseEvent();

    ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)mRenderer->Get_FrameBufferObject("scene")->Get_TextureId("main"), size, ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FileSystem_Obj"))
        {
            Message* message = (Message*)payload->Data;
       
            Entity* entity = new Entity();
            entity->AddComponent(new TransformComponent());   
            entity->AddComponent(new MeshComponent());         
            entity->GetComponent<MeshComponent>()->AttachRenderable(Model::LoadModel(std::string((const char*)message->data)));
            mRenderer->Get_Scene()->AttachEntity(entity);
            
        }
        ImGui::EndDragDropTarget();
    }

    if (size.x != mViewPortSize.x || size.y != mViewPortSize.y)
    {
        mViewPortResize = true;
        mViewPortSize = size;
    }

    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 2 - 50, 25));
    if (ImGui::ImageButton((void*)playButtonImage->Get_TextureId(), ImVec2(32, 32)))
    {
        std::ofstream out("Assets/Scenes/CurrentScene.json");
        json data = mRenderer->Ref_Scene()->SaveToJson();
        out << data.dump(4);
        out.close();

        for (auto [id, entity] : Entity::ALL_ENTITIES)
        {
            if (entity->HasComponent<ScriptComponent>())
                entity->GetComponent<ScriptComponent>()->DeleteScripts();
        }

        ScriptComponent::LOAD_DLL();

        for (auto [id, entity] : Entity::ALL_ENTITIES)
        {
            if (entity->HasComponent<ScriptComponent>())
                entity->GetComponent<ScriptComponent>()->ReloadScripts(entity);
        }

        mRenderer->Ref_Scene()->Ref_IsUpdating() = true;
    }

    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 2, 25));
    if (ImGui::ImageButton((void*)pauseButtonImage->Get_TextureId(), ImVec2(32, 32)))
    {
        mRenderer->Ref_Scene()->Ref_IsUpdating() = !mRenderer->Ref_Scene()->Ref_IsUpdating();
    }

    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 2 + 50, 25));
    if (ImGui::ImageButton((void*)stopButtonImage->Get_TextureId(), ImVec2(32, 32)))
    {
        delete mRenderer->Ref_Scene();
        std::ifstream in("Assets/Scenes/CurrentScene.json");
        json data = json::parse(in);
        in.close();
        mRenderer->Ref_Scene() = new Scene(data["Scene"]);
    }

    RenderGizmos();

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        int mouseX = ImGui::GetMousePos().x - ImGui::GetWindowContentRegionMin().x - ImGui::GetWindowPos().x;
        int mouseY = ImGui::GetMousePos().y - ImGui::GetWindowContentRegionMin().y - ImGui::GetWindowPos().y;
        int contentRegionX = ImGui::GetContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;
        int contentRegionY = ImGui::GetContentRegionMax().y - ImGui::GetWindowContentRegionMin().y;
        mouseY = contentRegionY - mouseY;

        if (mouseX >= 0 && mouseX <= contentRegionX &&
            mouseY >= 0 && mouseY <= contentRegionY)
        {
            //Todo 
            if (!ImGuizmo::IsUsing() && ImGui::IsWindowHovered())
            {
                std::any pixelData = mRenderer->Get_FrameBufferObject("scene")->ReadPixelData("pick", mouseX, mouseY);
                unsigned int id = std::any_cast<unsigned int>(pixelData);
            
                std::cout << id << std::endl;
                std::cout << "Attach Active Entity" << std::endl;

                //Todo : some probelm hovering imguizmo

                if (id != 0)
                    mRenderer->Get_Scene()->Ref_ActiveEntity() = Entity::ALL_ENTITIES.at(id);
                else
                    mRenderer->Get_Scene()->Ref_ActiveEntity() = nullptr;
            }           
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();
}

void Interface::RenderGizmos()
{
    if (mRenderer->Get_Scene()->Get_ActiveEntity() != nullptr)
    {
        static ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
        static ImGuizmo::MODE currentMode = ImGuizmo::LOCAL;
        auto transform = mRenderer->Get_Scene()->Get_ActiveEntity()->GetComponent<TransformComponent>();

        glm::mat4 viewMatrix = mRenderer->Get_Camera()->GetViewMatrix();
        glm::mat4 projectionMatrix = mRenderer->Get_Camera()->GetProjMatrix();
        auto parentTransform = mRenderer->Get_Scene()->Get_ActiveEntity()->Get_ParentTransformMatrix();
        auto meshTransform = parentTransform * transform->Get_TransformMatrix();

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
        ImGuizmo::SetDrawlist();
        ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), currentOperation, currentMode, glm::value_ptr(meshTransform), NULL, NULL, NULL);

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 translation, scale, rotation;
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(meshTransform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));

            glm::vec3 parentTranslation, parentScale, parentRotation;
            ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(parentTransform * transform->Get_TransformMatrix()), glm::value_ptr(parentTranslation), glm::value_ptr(parentRotation), glm::value_ptr(parentScale));

            switch (currentOperation)
            {
            case ImGuizmo::TRANSLATE:
                transform->Ref_Translation() += translation - parentTranslation;
                break;
            case ImGuizmo::ROTATE:
                //TODO: 180 degree only
                transform->Ref_Rotation() += rotation - parentRotation;
                break;
            case ImGuizmo::SCALE:
                transform->Ref_Scale() += scale - parentScale;
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
void Interface::RenderComponentsWindow()
{
    ImGui::Begin("Components");
    auto entity = mRenderer->Get_Scene()->Get_ActiveEntity();
    if (entity != nullptr)
    {
        if (entity->HasComponent<TransformComponent>())
            DrawTransformComponentUI(entity->GetComponent<TransformComponent>());    

        if (entity->HasComponent<MeshComponent>())
            DrawMeshComponentUI(entity->GetComponent<MeshComponent>());

        if (entity->HasComponent<LightComponent>())
            DrawLightComponentUI(entity->GetComponent<LightComponent>());
        
        if (entity->HasComponent<ScriptComponent>())
            DrawScriptComponentUI(entity->GetComponent<ScriptComponent>());
    }

    ImGui::Separator();

    ImGui::SetCursorPos(ImVec2(15, ImGui::GetCursorPos().y));
    if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionAvail().x - 15, 30)))
    {
        ImGui::OpenPopup("Select Component");
    }

    static int selected_fish = -1;
    const char* names[] = { "Transform", "Mesh Renderer", "Light", "Script" };

    if (ImGui::BeginPopup("Select Component"))
    {
        for (int i = 0; i < IM_ARRAYSIZE(names); i++)
        {
            if (ImGui::Selectable(names[i]))
            {
                if (names[i] == "Transform" && !entity->HasComponent<TransformComponent>())
                {
                    entity->AddComponent(new TransformComponent);
                }

                if (names[i] == "Script")
                {
                    scriptEntity = entity;
                }
            }
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}

void Interface::RenderSettingsWindow()
{
    ImGui::Begin("Settings");

    if (ImGui::CollapsingHeader("Scene"))
    {
        if (ImGui::CollapsingHeader("Wireframe"))
        {
            ImGui::SeparatorText("Points");
            ImGui::Text("Render");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::Checkbox("##RenderPoints", &mRenderer->GetRenderWireframePointsRef());
            ImGui::Text("Color");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::ColorEdit3("##ColorPoints", &mRenderer->GetWireframePointsColorRef()[0]);

            ImGui::SeparatorText("Lines");
            ImGui::Text("Render");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::Checkbox("##RenderLines", &mRenderer->GetRenderWireframeLinesRef());
            ImGui::Text("Color");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::ColorEdit3("##ColorLines", &mRenderer->GetWireframeLinesColorRef()[0]);

            ImGui::SeparatorText("Normals");
            ImGui::Text("Render");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::Checkbox("##RenderNormals", &mRenderer->GetRenderWireframeNormalsRef());
            ImGui::Text("Color");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::ColorEdit3("##ColorNormals", &mRenderer->GetWireframeNormalsColorRef()[0]);
        }
    }

    ImGui::DragFloat("Height Scale", &mRenderer->heightScale, 0.001);

    ImGui::End();
}
void Interface::Camera_KeyboardEvent()
{
    //W
    if (ImGui::IsKeyPressed(ImGuiKey_W))
    {
        mRenderer->Get_Camera()->Keyboard_ButtonEvent(GLFW_KEY_W, 0, GLFW_PRESS, 0);
    }
    if (ImGui::IsKeyReleased(ImGuiKey_W))
    {
        mRenderer->Get_Camera()->Keyboard_ButtonEvent(GLFW_KEY_W, 0, GLFW_RELEASE, 0);
    }

    //S
    if (ImGui::IsKeyPressed(ImGuiKey_S))
    {
        mRenderer->Get_Camera()->Keyboard_ButtonEvent(GLFW_KEY_S, 0, GLFW_PRESS, 0);
    }
    if (ImGui::IsKeyReleased(ImGuiKey_S))
    {
        mRenderer->Get_Camera()->Keyboard_ButtonEvent(GLFW_KEY_S, 0, GLFW_RELEASE, 0);
    }

    //A
    if (ImGui::IsKeyPressed(ImGuiKey_A))
    {
        mRenderer->Get_Camera()->Keyboard_ButtonEvent(GLFW_KEY_A, 0, GLFW_PRESS, 0);
    }
    if (ImGui::IsKeyReleased(ImGuiKey_A))
    {
        mRenderer->Get_Camera()->Keyboard_ButtonEvent(GLFW_KEY_A, 0, GLFW_RELEASE, 0);
    }

    //D
    if (ImGui::IsKeyPressed(ImGuiKey_D))
    {
        mRenderer->Get_Camera()->Keyboard_ButtonEvent(GLFW_KEY_D, 0, GLFW_PRESS, 0);
    }
    if (ImGui::IsKeyReleased(ImGuiKey_D))
    {
        mRenderer->Get_Camera()->Keyboard_ButtonEvent(GLFW_KEY_D, 0, GLFW_RELEASE, 0);
    }
}
void Interface::Camera_MouseEvent()
{
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
    {
        mRenderer->Get_Camera()->Mouse_ClickEvent(GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 0);
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle))
    {
        mRenderer->Get_Camera()->Mouse_ClickEvent(GLFW_MOUSE_BUTTON_MIDDLE, GLFW_RELEASE, 0);
    }
    if (ImGui::IsWindowHovered())
    {
        mRenderer->Get_Camera()->Mouse_MoveEvent(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    }
}
void Interface::DisplayEntity(Entity* entity)
{
    static int selectedNode = -1;

    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (selectedNode == entity->Get_Id())
        node_flags |= ImGuiTreeNodeFlags_Selected;
    bool open = ImGui::TreeNodeEx(entity->Get_Name().c_str(), node_flags);

    if (ImGui::IsItemClicked())
    {
        selectedNode = entity->Get_Id();
        mRenderer->Get_Scene()->Ref_ActiveEntity() = entity;
    }

    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Delete"))
        {
            mRenderer->Get_Scene()->AddToDelete(entity);
            
            if (entity == mRenderer->Get_Scene()->Get_ActiveEntity())
            {
                selectedNode = -1;
                mRenderer->Get_Scene()->Ref_ActiveEntity() = nullptr;
                //Todo error
            }
        }

        if (ImGui::MenuItem("Duplicate"))
        {
            mRenderer->Get_Scene()->AddToCopy(entity);
        }

        ImGui::EndPopup();
    }

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("Entity_Tree", &entity->Ref_Id(), sizeof(unsigned int));
        ImGui::Text(entity->Get_Name().c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity_Tree"))
        {
            unsigned int id = *(unsigned int*)payload->Data;
            Entity* droppedEntity = Entity::ALL_ENTITIES[id];
            
            if (droppedEntity->Get_Parent() == nullptr)
                mRenderer->Get_Scene()->DetachEntity(droppedEntity);
            else
                droppedEntity->Ref_Parent()->DetachChild(droppedEntity);

            droppedEntity->Ref_Parent() = entity;
            entity->AttachChild(droppedEntity);

            //TODO CRASH IF PARENT GOES TO CHILD
        }
        ImGui::EndDragDropTarget();
    }

    if (open)
    {
        for (auto obj : entity->Get_Children())
        {
            DisplayEntity(obj);
        }

        ImGui::TreePop();
    }
}
void Interface::EntityWindow()
{
    ImGui::Begin("Entities");

    for (auto entity : mRenderer->Get_Scene()->Get_EntityList())
    {
        DisplayEntity(entity);
    }

    if (ImGui::GetContentRegionAvail().x > 0 && ImGui::GetContentRegionAvail().y > 0)
    {
        ImGui::InvisibleButton("##invisible", ImGui::GetContentRegionAvail());

        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Empty"))
            {
                Entity* entity = new Entity;
                entity->AddComponent(new TransformComponent());
                mRenderer->Get_Scene()->AttachEntity(entity);
            }
            if (ImGui::BeginMenu("Shape 2D"))
            {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Shape 3D"))
            {
            
                if (ImGui::MenuItem("Cube"))
                {
                    Entity* entity = new Entity;
                    entity->Ref_Name() = "Cube##" + std::to_string(entity->Get_Id());
                    entity->AddComponent(new TransformComponent);
                    entity->AddComponent(new MeshComponent);
                    entity->GetComponent<MeshComponent>()->AttachRenderable(new Cube);
                    mRenderer->Get_Scene()->AttachEntity(entity);
                }
                if (ImGui::MenuItem("Pyramid"))
                {
                    Entity* entity = new Entity;
                    entity->Ref_Name() = "Pyramid##" + std::to_string(entity->Get_Id());
                    entity->AddComponent(new TransformComponent);
                    entity->AddComponent(new MeshComponent);
                    entity->GetComponent<MeshComponent>()->AttachRenderable(new Pyramid);
                    mRenderer->Get_Scene()->AttachEntity(entity);
                }
                if (ImGui::MenuItem("Cylinder"))
                {
                    Entity* entity = new Entity;
                    entity->Ref_Name() = "Cylinder##" + std::to_string(entity->Get_Id());
                    entity->AddComponent(new TransformComponent);
                    entity->AddComponent(new MeshComponent);
                    entity->GetComponent<MeshComponent>()->AttachRenderable(new Cylinder);
                    mRenderer->Get_Scene()->AttachEntity(entity);
                }
                if (ImGui::MenuItem("Capsule"))
                {
                    Entity* entity = new Entity;
                    entity->Ref_Name() = "Capsule##" + std::to_string(entity->Get_Id());
                    entity->AddComponent(new TransformComponent);
                    entity->AddComponent(new MeshComponent);
                    entity->GetComponent<MeshComponent>()->AttachRenderable(new Capsule);
                    mRenderer->Get_Scene()->AttachEntity(entity);
                }
                if (ImGui::MenuItem("Sphere"))
                {
                    Entity* entity = new Entity;
                    entity->Ref_Name() = "Sphere##" + std::to_string(entity->Get_Id());
                    entity->AddComponent(new TransformComponent);
                    entity->AddComponent(new MeshComponent);
                    entity->GetComponent<MeshComponent>()->AttachRenderable(new Sphere);
                    mRenderer->Get_Scene()->AttachEntity(entity);
                }
                if (ImGui::MenuItem("Torus"))
                {
                    Entity* entity = new Entity;
                    entity->Ref_Name() = "Torus##" + std::to_string(entity->Get_Id());
                    entity->AddComponent(new TransformComponent);
                    entity->AddComponent(new MeshComponent);
                    entity->GetComponent<MeshComponent>()->AttachRenderable(new Torus);
                    mRenderer->Get_Scene()->AttachEntity(entity);
                }
            
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Lights"))
            {
            
                if (ImGui::MenuItem("Direction"))
                {
                    Entity* entity = new Entity;
                    entity->Ref_Name() = "Direction Light##" + std::to_string(entity->Get_Id());
                    entity->AddComponent(new TransformComponent);
                    entity->AddComponent(new MeshComponent);
                    entity->AddComponent(new LightComponent);
                    entity->GetComponent<MeshComponent>()->AttachRenderable(new Cube);
                    entity->GetComponent<LightComponent>()->AttachLight(new DirectionLight);
                    entity->GetComponent<TransformComponent>()->Ref_Scale() = glm::vec3(0.15);
                    mRenderer->Get_Scene()->AttachEntity(entity);
                }
                if (ImGui::MenuItem("Point"))
                {
                    Entity* entity = new Entity;
                    entity->Ref_Name() = "Point Light##" + std::to_string(entity->Get_Id());
                    entity->AddComponent(new TransformComponent);
                    entity->AddComponent(new MeshComponent);
                    entity->AddComponent(new LightComponent);
                    entity->GetComponent<MeshComponent>()->AttachRenderable(new Sphere);
                    entity->GetComponent<LightComponent>()->AttachLight(new PointLight);
                    entity->GetComponent<TransformComponent>()->Ref_Scale() = glm::vec3(0.15);
                    mRenderer->Get_Scene()->AttachEntity(entity);
                }
            
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity_Tree"))
            {
                unsigned int id = *(unsigned int*)payload->Data;
                Entity* droppedEntity = Entity::ALL_ENTITIES[id];

                if (droppedEntity->Get_Parent() != nullptr)
                {
                    droppedEntity->Ref_Parent()->DetachChild(droppedEntity);
                    droppedEntity->Ref_Parent() = nullptr;
                    mRenderer->Get_Scene()->AttachEntity(droppedEntity);           
                }

            }
            ImGui::EndDragDropTarget();
        }
    }

    ImGui::End();
}
void Interface::FileSystemWindow()
{
    static std::string data;

    ImGui::Begin("FileSystem");
    static std::filesystem::path nextDir = std::filesystem::current_path();

    std::filesystem::path currentDir = nextDir;
    std::filesystem::directory_iterator directory = std::filesystem::directory_iterator(currentDir);

    int columns = glm::max(ImGui::GetContentRegionAvail().x / 80.f, 1.f);
    int count = 0;

    if (ImGui::BeginTable("##FileSystemTable", columns, ImGuiTableColumnFlags_NoResize))
    {
        ImGui::TableNextColumn();
        if (ImGui::ImageButton("parent", (void*)parentImage->Get_TextureId(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0)))
        {
            nextDir = currentDir.parent_path();
        }
        count++;

        for (const auto& entry : directory)
        {
            if (count == columns)
            {
                count = 0;
                ImGui::TableNextRow();
            }

            if (entry.is_directory())
            {
                ImGui::TableNextColumn();
                if (ImGui::ImageButton(entry.path().string().c_str(), (void*)folderImage->Get_TextureId(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0)))
                {
                    nextDir = entry.path();
                }
                ImGui::Text(entry.path().filename().string().c_str());
                count++;
            }
        }

        directory = std::filesystem::directory_iterator(currentDir);
        for (const auto& entry : directory)
        {
            if (count == columns)
            {
                count = 0;
                ImGui::TableNextRow();
            }

            if (entry.is_regular_file())
            {
                unsigned int textureId = fileImage->Get_TextureId();

                if (entry.path().extension() == ".jpg" || entry.path().extension() == ".png" || entry.path().extension() == ".jpeg")
                {
                    textureId = ImageTexture::LoadTexture(entry.path().string())->Get_TextureId();
                }

                ImGui::TableNextColumn();
                ImGui::ImageButton(entry.path().string().c_str(), (void*)textureId, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
                
                if (entry.path().extension().string() == ".png" || entry.path().extension().string() == ".jpg" || entry.path().extension().string() == ".jpeg")
                {
                    if (ImGui::BeginDragDropSource())
                    {
                        data = entry.path().string();

                        Message message;
                        message.type = MessageType::TEXTURE;
                        message.data = data.c_str();

                        ImGui::SetDragDropPayload("FileSystem_Image", &message, sizeof(message));
                        ImGui::Text(entry.path().filename().string().c_str());
                        ImGui::EndDragDropSource();
                    }
                }

                if (entry.path().extension().string() == ".obj")
                {
                    if (ImGui::BeginDragDropSource())
                    {
                        data = entry.path().string();

                        Message message;
                        message.type = MessageType::TEXTURE;
                        message.data = data.c_str();

                        ImGui::SetDragDropPayload("FileSystem_Obj", &message, sizeof(message));
                        ImGui::Text(entry.path().filename().string().c_str());
                        ImGui::EndDragDropSource();
                    }
                }

                ImGui::Text(entry.path().filename().string().c_str());
                count++;

            }
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

void Interface::DrawScriptComponentUI(ScriptComponent* scriptComponent)
{
    if (ImGui::CollapsingHeader("Script Component", ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (auto [name, script] : scriptComponent->Get_ScriptList())
        {
            ImGui::Text(name.c_str());
            ImGui::SameLine();
            ImGui::Text("%d", script);
        }
    }
}

void Interface::DrawMeshComponentUI(MeshComponent* meshComponent)
{
    static std::string label;

    if (ImGui::CollapsingHeader("Mesh Component", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SeparatorText("Surface Settings");

        DrawLeftLabel("Hard Surface");
        ImGui::Checkbox("##hardsurface", &meshComponent->Ref_HardSurface());

        if (auto sphere = dynamic_cast<Sphere*>(meshComponent->Get_Renderable()))
        {
            DrawLeftLabel("Count");
            if (ImGui::DragInt("##count", &sphere->Ref_Count(), 0.05f, 3, 100))
                sphere->ReGenerate();
        }

        if (auto cylinder = dynamic_cast<Cylinder*>(meshComponent->Get_Renderable()))
        {
            DrawLeftLabel("Count");
            if (ImGui::DragInt("##count", &cylinder->Ref_Count(), 0.05f, 3, 100))
                cylinder->ReGenerate();
            
            DrawLeftLabel("Radius Top");
            if (ImGui::DragFloat("##radiusTop", &cylinder->Ref_RadiusTop(), 0.05f))
                cylinder->ReGenerate();

            DrawLeftLabel("Radius Bot");
            if (ImGui::DragFloat("##radiusBot", &cylinder->Ref_RadiusBottom(), 0.05f))
                cylinder->ReGenerate();
        }

        ImGui::SeparatorText("Color Settings");
        DrawColorEdit3("Ambient", meshComponent->Ref_Material().ambient);
        DrawColorEdit3("Diffuse", meshComponent->Ref_Material().diffuse);
        DrawColorEdit3("Specular", meshComponent->Ref_Material().specular);

        ImGui::SeparatorText("Texture Settings");
        DrawDragFloat("Scale", meshComponent->Ref_Textures().scale, 0.01f);
        DrawDragFloat("ScaleX", meshComponent->Ref_Textures().scaleX, 0.01f);
        DrawDragFloat("ScaleY", meshComponent->Ref_Textures().scaleY, 0.01f);

        label = meshComponent->Get_Textures().texture == nullptr ? "##Diffuse" : std::filesystem::path(meshComponent->Get_Textures().texture->Get_Path()).filename().string();
        DrawLeftLabel("Diffuse");
        DrawButton(label, ImVec2(ImGui::GetContentRegionAvail().x - 35, 17));
        AttachDropTarget("FileSystem_Image", AcceptDroppedDiffuseTexture);
        ImGui::SameLine();
        if (ImGui::Button("...##1", ImVec2(30, 17)))
        {
            ImGui::OpenPopup("DiffuseTextureOption");
        }

        label = meshComponent->Get_Textures().normal == nullptr ? "##Normal" : std::filesystem::path(meshComponent->Get_Textures().normal->Get_Path()).filename().string();
        DrawLeftLabel("Normal");
        DrawButton(label, ImVec2(ImGui::GetContentRegionAvail().x - 35, 17));
        AttachDropTarget("FileSystem_Image", AcceptDroppedNormalTexture);
        ImGui::SameLine();
        if (ImGui::Button("...##2", ImVec2(30, 17)))
        {
            ImGui::OpenPopup("NormalTextureOption");
        }

        label = meshComponent->Get_Textures().height == nullptr ? "##Height" : std::filesystem::path(meshComponent->Get_Textures().height->Get_Path()).filename().string();
        DrawLeftLabel("Height");
        DrawButton(label, ImVec2(ImGui::GetContentRegionAvail().x - 35, 17));
        AttachDropTarget("FileSystem_Image", AcceptDroppedHeightTexture);
        ImGui::SameLine();
        if (ImGui::Button("...##3", ImVec2(30, 17)))
        {
            ImGui::OpenPopup("HeightTextureOption");
        }

        if (ImGui::BeginPopup("DiffuseTextureOption"))
        {
            if (ImGui::MenuItem("Load"))
            {

            }
            if (ImGui::MenuItem("Delete"))
            {
                meshComponent->Ref_Textures().texture = nullptr;
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopup("NormalTextureOption"))
        {
            if (ImGui::MenuItem("Load"))
            {

            }
            if (ImGui::MenuItem("Delete"))
            {
                meshComponent->Ref_Textures().normal = nullptr;
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopup("HeightTextureOption"))
        {
            if (ImGui::MenuItem("Load"))
            {
            }
            if (ImGui::MenuItem("Delete"))
            {
                meshComponent->Ref_Textures().height = nullptr;
            }
            ImGui::EndPopup();
        }
    }
}

void Interface::AttachDropTarget(const std::string& acceptText, std::function<void(const void* data)> callback)
{
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(acceptText.c_str()))
        {
            callback(payload->Data);
        }
        ImGui::EndDragDropTarget();
    }
}
void Interface::AcceptDroppedDiffuseTexture(const void* data)
{
    Message* message = (Message*)data;
    mRenderer->Get_Scene()->Ref_ActiveEntity()->GetComponent<MeshComponent>()->Ref_Textures().texture = ImageTexture::LoadTexture(std::string((const char*)message->data));
}
void Interface::AcceptDroppedNormalTexture(const void* data)
{
    Message* message = (Message*)data;
    mRenderer->Get_Scene()->Ref_ActiveEntity()->GetComponent<MeshComponent>()->Ref_Textures().normal = ImageTexture::LoadTexture(std::string((const char*)message->data));
}
void Interface::AcceptDroppedHeightTexture(const void* data)
{
    Message* message = (Message*)data;
    mRenderer->Get_Scene()->Ref_ActiveEntity()->GetComponent<MeshComponent>()->Ref_Textures().height = ImageTexture::LoadTexture(std::string((const char*)message->data));

}
void Interface::DrawButton(const std::string& text, const ImVec2& size, std::function<void()> callback)
{
    if (ImGui::Button(text.c_str(), size))
    {
        callback();
    }
}
void Interface::asdasd()
{
    ImGui::OpenPopup("Mesh Texture Settings");
}

void Interface::DrawColorEdit3(const std::string& text, glm::vec3& color)
{
    DrawLeftLabel(text);
    ImGui::ColorEdit3(std::string("##" + text).c_str(), &color.x, 0.05f);
}
void Interface::DrawLeftLabel(const std::string& text, int width)
{
    ImGui::Text(text.c_str());
    ImGui::SameLine();
    ImGui::SetCursorPos(ImVec2(width, ImGui::GetCursorPos().y));
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
}

void Interface::DrawDragFloat(const std::string& text, float& value, float speed)
{
    DrawLeftLabel(text);
    ImGui::DragFloat(std::string("##" + text).c_str(), &value, speed);
}

void Interface::DrawLightComponentUI(LightComponent* lightComponent)
{
    if (ImGui::CollapsingHeader("Light Component", ImGuiTreeNodeFlags_DefaultOpen))
    {
        auto lightSource = lightComponent->Ref_LightSource();

        ImGui::SeparatorText("Light Material");

        DrawLeftLabel("Color");
        ImGui::ColorEdit3("##ColorLightComponent", glm::value_ptr(lightSource->Ref_Color()));

        DrawLeftLabel("Diffuse");
        ImGui::DragFloat("##DiffuseLightComponent", &lightSource->Ref_DiffuseIntensity(), 0.001f, 0, 1);

        DrawLeftLabel("Specular");
        ImGui::DragFloat("##SpecularLightComponent", &lightSource->Ref_SpecularIntensity(), 0.001f, 0, 1);

        DrawLeftLabel("Shadow");
        ImGui::Checkbox("##ShadowLightComponent", &lightSource->Ref_CastShadow());

        if (auto directionLight = dynamic_cast<DirectionLight*>(lightSource))
        {
            ImGui::SeparatorText("Direction Light");

            DrawLeftLabel("Direction");
            ImGui::DragFloat3("##DirectionLightComponent", glm::value_ptr(directionLight->Ref_Direction()), 0.001f, -1, 1);

            DrawLeftLabel("ShadowBox X");
            ImGui::DragFloat2("##ShadowBoxXLightComponent", glm::value_ptr(directionLight->Ref_ShadowBoxX()), 0.01f);

            DrawLeftLabel("ShadowBox Y");
            ImGui::DragFloat2("##ShadowBoxYLightComponent", glm::value_ptr(directionLight->Ref_ShadowBoxY()), 0.01f);

            DrawLeftLabel("ShadowBox Z");
            ImGui::DragFloat2("##ShadowBoxZLightComponent", glm::value_ptr(directionLight->Ref_ShadowBoxZ()), 0.01f);

            ImGui::SeparatorText("Shadow Map");
            ImGui::Image((void*)mRenderer->Get_FrameBufferObject("shadow")->Get_TextureId("shadow"), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x), ImVec2(0, 1), ImVec2(1, 0));
        }

        else if (auto pointLight = dynamic_cast<PointLight*>(lightSource))
        {
            ImGui::SeparatorText("Point Light");

            DrawLeftLabel("Poisiton");
            ImGui::DragFloat3("##DirectionLightComponent", glm::value_ptr(pointLight->Ref_Position()), 0.01f);
        }
    }
}

void Interface::DrawTransformComponentUI(TransformComponent* transformComponent)
{
    if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen))
    {
        DrawLeftLabel("Translation");
        ImGui::DragFloat3("##Translation##TransformComponent", glm::value_ptr(transformComponent->Ref_Translation()), 0.05f);

        DrawLeftLabel("Rotation");
        ImGui::DragFloat3("##Rotation##TransformComponent", glm::value_ptr(transformComponent->Ref_Rotation()), 0.05f);

        DrawLeftLabel("Scale");
        ImGui::DragFloat3("##Scale##TransformComponent", glm::value_ptr(transformComponent->Ref_Scale()), 0.05f);
    }
}