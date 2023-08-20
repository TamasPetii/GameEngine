#include "Interface.h"

Interface::Interface(GLFWwindow* window, Renderer* renderer)
{
	mWindow = window;
	mRenderer = renderer;

    Texture2D::LoadTexture2D("Assets/Gui/ViewPort/Pause.png");
    Texture2D::LoadTexture2D("Assets/Gui/ViewPort/Play.png");
    Texture2D::LoadTexture2D("Assets/Gui/ViewPort/Stop.png");
    Texture2D::LoadTexture2D("Assets/Gui/FileSystem/File.png");
    Texture2D::LoadTexture2D("Assets/Gui/FileSystem/Directory.png");
    Texture2D::LoadTexture2D("Assets/Gui/FileSystem/Parent.png");

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
    mRenderer->GetCamera()->Update();

    if (mViewPortResize)
    {
        mViewPortResize = false;
        mRenderer->GetCamera()->SetProjMatrix(mViewPortSize.x, mViewPortSize.y);
        mRenderer->GetSceneFrameBuffer()->ResizeBuffers(mViewPortSize.x, mViewPortSize.y);
        mRenderer->GetItemPickFrameBuffer()->ResizeBuffers(mViewPortSize.x, mViewPortSize.y);
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
                //TODO
            }
            if (ImGui::MenuItem("Save"))
            {
                //TODO
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void Interface::RenderViewPortWindow()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("ViewPort", nullptr, ImGuiWindowFlags_NoCollapse);

    Camera_KeyboardEvent();
    Camera_MouseEvent();

    ImVec2 size = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)mRenderer->GetSceneFrameBuffer()->GetTextureId(), size, ImVec2(0, 1), ImVec2(1, 0));

    if (size.x != mViewPortSize.x || size.y != mViewPortSize.y)
    {
        mViewPortResize = true;
        mViewPortSize = size;
    }
    
    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 2 - 50, 25));
    ImGui::ImageButton((void*)Texture2D::GetTexture2D("Assets/Gui/ViewPort/Play.png")->GetTextureId(), ImVec2(32, 32));

    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 2, 25));
    ImGui::ImageButton((void*)Texture2D::GetTexture2D("Assets/Gui/ViewPort/Pause.png")->GetTextureId(), ImVec2(32, 32));

    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x / 2 + 50, 25));
    ImGui::ImageButton((void*)Texture2D::GetTexture2D("Assets/Gui/ViewPort/Stop.png")->GetTextureId(), ImVec2(32, 32));

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
            int id = dynamic_cast<FrameBufferObject<FBO_IntegerTexture>*>(mRenderer->GetItemPickFrameBuffer())->ReadPixelData(mouseX, mouseY);

            if (!ImGuizmo::IsUsing() && !ImGuizmo::IsOver() && ImGui::IsWindowHovered() && !mRenderer->FindActiveEntity(id))
            {
                mRenderer->NoActiveObject();
            }
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();
}

void Interface::RenderGizmos()
{
    if (mRenderer->GetActiveEntity() == nullptr) return;

    static ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
    static ImGuizmo::MODE currentMode = ImGuizmo::WORLD;
    TransformComponent* transform = mRenderer->GetActiveEntity()->GetComponent<TransformComponent>();

    glm::mat4 viewMatrix = mRenderer->GetCamera()->GetViewMatrix();
    glm::mat4 projectionMatrix = mRenderer->GetCamera()->GetProjMatrix();
    glm::mat4 cubeTransform = transform->GetTransformMatrix();

    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
    ImGuizmo::SetDrawlist();
    ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), currentOperation, currentMode, glm::value_ptr(cubeTransform), NULL, NULL);

    if (ImGuizmo::IsUsing())
    {
        glm::vec3 translation, scale, rotation;
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(cubeTransform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));

        switch (currentOperation)
        {
        case ImGuizmo::TRANSLATE:
            transform->GetTranslation() = translation;
            break;
        case ImGuizmo::ROTATE:
            glm::vec3 rotationDelta = transform->GetRotation() - rotation;
            transform->GetRotation() += rotation;
            break;
        case ImGuizmo::SCALE:
            transform->GetScale() = scale;
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

void Interface::RenderComponentsWindow()
{
    ImGui::Begin("Components");
    auto entity = mRenderer->GetActiveEntity();
    if (entity != nullptr)
    {
        if (entity->HasComponent<TransformComponent>() && ImGui::CollapsingHeader("Transformation", ImGuiTreeNodeFlags_DefaultOpen))
        {
            TransformComponent* transform = entity->GetComponent<TransformComponent>();

            std::string translateLabel = "##Translate" + std::to_string(entity->GetId());
            ImGui::Text("Translation");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::DragFloat3(translateLabel.c_str(), &transform->GetTranslation().x, 0.05f);

            std::string rotationLabel = "##Rotate" + std::to_string(entity->GetId());
            ImGui::Text("Rotation");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::DragFloat3(rotationLabel.c_str(), &transform->GetRotation().x, 0.05f);

            std::string scaleLabel = "##Scale" + std::to_string(entity->GetId());
            ImGui::Text("Scale");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::DragFloat3(scaleLabel.c_str(), &transform->GetScale().x, 0.05f);
        }

        if (entity->HasComponent<MeshComponent>() && ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
        {
            MeshComponent* mesh = entity->GetComponent<MeshComponent>();

            if (dynamic_cast<Shape<Cylinder>*>(mesh->GetMesh()))
            {
                bool change = false;

                Shape<Cylinder>* cylinder = dynamic_cast<Shape<Cylinder>*>(mesh->GetMesh());
                cylinder->GetLayout();

                ImGui::Text("Points");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                if(ImGui::DragInt("##Points", &cylinder->GetLayout().mCount, 0.05f, 3)) change = true;

                ImGui::Text("Height");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::DragFloat("##Height", &cylinder->GetLayout().mHeight, 0.05f, 0)) change = true;

                ImGui::Text("Radius T");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::DragFloat("##RadiusTop", &cylinder->GetLayout().mRadiusTop, 0.05f, 0)) change = true;

                ImGui::Text("Radius B");
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                if (ImGui::DragFloat("##RadiusBottom", &cylinder->GetLayout().mRadiusBottom, 0.05f, 0)) change = true;

                if (change)
                {
                    change = false;
                    cylinder->LoadVertices();
                    cylinder->HardNormals();
                    cylinder->LoadIndices();
                    cylinder->RefreshShape();
                }
            }

            ImGui::SeparatorText("ASD");

            std::vector<std::string> items = Texture2D::GetTextureNames();
            std::vector<const char*> c_items;
            c_items.reserve(items.size());
            for (const auto& item : items)
            {
                c_items.push_back(item.c_str());
            }

            int item_current = 0;
            for (int i = 1; i < items.size(); i++)
            {
                if (mesh->GetTexture() != nullptr && items[i] == mesh->GetTexture()->GetPath()) item_current = i;
            }

            std::string textureLabel = "##Texture" + std::to_string(entity->GetId());
            ImGui::Text("Texture");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::Combo("combo", &item_current, c_items.data(), static_cast<int>(c_items.size())))
            {
                if (item_current != 0)
                    mesh->SetTexture(Texture2D::LoadTexture2D(items[item_current]));
                else
                    mesh->SetTexture(nullptr);
            }

            std::string colorLabel = "##Color" + std::to_string(entity->GetId());
            ImGui::Text("Color");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::ColorEdit3(colorLabel.c_str(), &mesh->GetColor().x, 0.05f);

            std::string shadeLabel = "##ShadeSmooth" + std::to_string(entity->GetId());
            ImGui::Text("Shade");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::Checkbox(shadeLabel.c_str(), &mesh->GetShadeSmooth()))
            {
                mesh->ChangeShade();
            }
        }

        if (entity->HasComponent<LightComponent>() && ImGui::CollapsingHeader("Light"))
        {
            LightComponent* light = entity->GetComponent<LightComponent>();
            
            ImGui::Text("Cast Shadows");
            ImGui::SameLine();
            if(ImGui::Checkbox("##shadow", &light->GetUseShadow()))
            {
                if (light->GetUseShadow())
                    mRenderer->GetShadowEntity() = entity;
                else
                    mRenderer->GetShadowEntity() = nullptr;
            }
            if (mRenderer->GetShadowEntity() != nullptr)
            {
                ImGui::Image((void*)mRenderer->GetShadowFrameBuffer()->GetTextureId(), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().x), ImVec2(0, 1), ImVec2(1, 0));
            }
        }
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
            if (ImGui::Selectable(names[i]) && names[i] == "Script")
            {
                ScriptComponent::GenerateScript("MyScript");
                std::cout << "Script" << std::endl;
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

    ImGui::End();
}

void Interface::Camera_KeyboardEvent()
{
    //W
    if (ImGui::IsKeyPressed(ImGuiKey_W))
    {
        mRenderer->GetCamera()->Keyboard_ButtonEvent(GLFW_KEY_W, 0, GLFW_PRESS, 0);
    }
    if (ImGui::IsKeyReleased(ImGuiKey_W))
    {
        mRenderer->GetCamera()->Keyboard_ButtonEvent(GLFW_KEY_W, 0, GLFW_RELEASE, 0);
    }

    //S
    if (ImGui::IsKeyPressed(ImGuiKey_S))
    {
        mRenderer->GetCamera()->Keyboard_ButtonEvent(GLFW_KEY_S, 0, GLFW_PRESS, 0);
    }
    if (ImGui::IsKeyReleased(ImGuiKey_S))
    {
        mRenderer->GetCamera()->Keyboard_ButtonEvent(GLFW_KEY_S, 0, GLFW_RELEASE, 0);
    }

    //A
    if (ImGui::IsKeyPressed(ImGuiKey_A))
    {
        mRenderer->GetCamera()->Keyboard_ButtonEvent(GLFW_KEY_A, 0, GLFW_PRESS, 0);
    }
    if (ImGui::IsKeyReleased(ImGuiKey_A))
    {
        mRenderer->GetCamera()->Keyboard_ButtonEvent(GLFW_KEY_A, 0, GLFW_RELEASE, 0);
    }

    //D
    if (ImGui::IsKeyPressed(ImGuiKey_D))
    {
        mRenderer->GetCamera()->Keyboard_ButtonEvent(GLFW_KEY_D, 0, GLFW_PRESS, 0);
    }
    if (ImGui::IsKeyReleased(ImGuiKey_D))
    {
        mRenderer->GetCamera()->Keyboard_ButtonEvent(GLFW_KEY_D, 0, GLFW_RELEASE, 0);
    }
}

void Interface::Camera_MouseEvent()
{
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle))
    {
        mRenderer->GetCamera()->Mouse_ClickEvent(GLFW_MOUSE_BUTTON_MIDDLE, GLFW_PRESS, 0);
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle))
    {
        mRenderer->GetCamera()->Mouse_ClickEvent(GLFW_MOUSE_BUTTON_MIDDLE, GLFW_RELEASE, 0);
    }
    if (ImGui::IsWindowHovered())
    {
        mRenderer->GetCamera()->Mouse_MoveEvent(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    }
}

void Interface::DisplayEntity(Entity* entity)
{
    static int selectedNode = -1;

    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (selectedNode == entity->GetId())
        node_flags |= ImGuiTreeNodeFlags_Selected;
    bool open = ImGui::TreeNodeEx(entity->GetText().c_str(), node_flags);

    if (ImGui::IsItemClicked())
    {
        std::cout << entity->GetId() << std::endl;
        selectedNode = entity->GetId();
        mRenderer->FindActiveEntity(entity->GetId());
    }

    if (ImGui::BeginPopupContextItem())
    {
        /*
        char buf[30];
        ImGui::Text("Edit Name");
        ImGui::InputText("##EntityName", buf, sizeof(buf));
        ImGui::SameLine();
        if (ImGui::Button("OK"))
        {
            std::cout << "Changed Name: " << buf << std::endl;
            entity->GetText() = buf;
        }

        ImGui::Separator();
        */

        if (ImGui::MenuItem("Delete"))
        {

        }

        ImGui::EndPopup();
    }
  
    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("TREE_NODE", &entity->GetId(), sizeof(int*));
        ImGui::Text(entity->GetText().c_str());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TREE_NODE"))
        {
            int* id = (int*)payload->Data;
            Entity* obj = Entity::Get(*id);
            if (obj != nullptr)
            {
                std::cout << "OBJ: " << obj->GetId() << std::endl;
                if (obj->GetParent() == nullptr)
                    mRenderer->AddToErase(obj);
                obj->Remove();
                entity->AddChild(obj);
                //TODO CRASH IF PARENT GOES TO CHILD
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (open)
    {
        for (auto obj : entity->GetChildren())
        {
            DisplayEntity(obj);
        }

        ImGui::TreePop();
    }
}

void Interface::EntityWindow()
{
    ImGui::Begin("Entities");

    for (auto entity : mRenderer->GetEntities())
    {
        DisplayEntity(entity);
    }

    ImGui::InvisibleButton("##invisible", ImGui::GetContentRegionAvail());

    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Empty"))
        {   
            Entity* entity = new Entity;
            entity->AddComponent(new TransformComponent());
            mRenderer->GetEntities().insert(entity);
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
                entity->AddComponent(new TransformComponent());
                MeshComponent* mesh = new MeshComponent();
                mesh->AttachMesh(new Shape<Cube>());
                entity->AddComponent(mesh);
                mRenderer->GetEntities().insert(entity);
                entity->GetText() = "Cube (" + std::to_string(entity->GetId()) + ")";
            }
            if (ImGui::MenuItem("Cylinder"))
            {
                Entity* entity = new Entity;
                entity->AddComponent(new TransformComponent());
                MeshComponent* mesh = new MeshComponent();
                mesh->AttachMesh(new Shape<Cylinder>());
                entity->AddComponent(mesh);
                mRenderer->GetEntities().insert(entity);
                entity->GetText() = "Cylinder (" + std::to_string(entity->GetId()) + ")";
            }
            if (ImGui::MenuItem("Sphere"))
            {
                Entity* entity = new Entity;
                entity->AddComponent(new TransformComponent());
                MeshComponent* mesh = new MeshComponent();
                mesh->AttachMesh(new Shape<Sphere>());
                entity->AddComponent(mesh);
                mRenderer->GetEntities().insert(entity);
                entity->GetText() = "Sphere (" + std::to_string(entity->GetId()) + ")";
            }
            if (ImGui::MenuItem("Torus"))
            {
                Entity* entity = new Entity;
                entity->AddComponent(new TransformComponent());
                MeshComponent* mesh = new MeshComponent();
                mesh->AttachMesh(new Shape<Torus>());
                entity->AddComponent(mesh);
                mRenderer->GetEntities().insert(entity);
                entity->GetText() = "Torus (" + std::to_string(entity->GetId()) + ")";
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Lights"))
        {
            if (ImGui::MenuItem("Direction"))
            {
                Entity* entity = new Entity;
                MeshComponent* mesh = new MeshComponent();
                TransformComponent* transform = new TransformComponent();
                LightComponent* light = new LightComponent();

                light->AttachLight(new Light<Directional>);
                mesh->AttachMesh(new Shape<Cube>());
                transform->GetScale() = glm::vec3(0.1);

                entity->AddComponent(mesh);
                entity->AddComponent(transform);
                entity->AddComponent(light);

                mRenderer->GetEntities().insert(entity);

                entity->GetText() = "Direction Light (" + std::to_string(entity->GetId()) + ")";
            }
            if (ImGui::MenuItem("Point"))
            {
                Entity* entity = new Entity;
                MeshComponent* mesh = new MeshComponent();
                TransformComponent* transform = new TransformComponent();
                LightComponent* light = new LightComponent();

                light->AttachLight(new Light<Point>);
                mesh->AttachMesh(new Shape<Cube>());
                transform->GetScale() = glm::vec3(0.1);

                entity->AddComponent(mesh);
                entity->AddComponent(transform);
                entity->AddComponent(light);

                mRenderer->GetEntities().insert(entity);

                entity->GetText() = "Point Light (" + std::to_string(entity->GetId()) + ")";
            }
            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TREE_NODE"))
        {
            int* id = (int*)payload->Data;
            Entity* obj = Entity::Get(*id);
            if (obj != nullptr)
            {
                obj->Remove();
                mRenderer->GetEntities().insert(obj);
            }
        }
        ImGui::EndDragDropTarget();
    } 

    ImGui::End();
}

void Interface::FileSystemWindow()
{
    ImGui::Begin("FileSystem");
    static std::filesystem::path nextDir = std::filesystem::current_path();

    std::filesystem::path currentDir = nextDir;
    std::filesystem::directory_iterator directory = std::filesystem::directory_iterator(currentDir);

    int columns = glm::max(ImGui::GetContentRegionAvail().x / 80.f, 1.f);
    int count = 0;

    if (ImGui::BeginTable("##FileSystemTable", columns, ImGuiTableColumnFlags_NoResize))
    {
        int textureIdParent = Texture2D::GetTexture2D("Assets/Gui/FileSystem/Parent.png")->GetTextureId();
        int textureIdFolder = Texture2D::GetTexture2D("Assets/Gui/FileSystem/Directory.png")->GetTextureId();     
        int textureIdFile = Texture2D::GetTexture2D("Assets/Gui/FileSystem/File.png")->GetTextureId();

        ImGui::TableNextColumn();
        if (ImGui::ImageButton("parent", (void*)textureIdParent, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0)))
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
                if (ImGui::ImageButton(entry.path().string().c_str(), (void*)textureIdFolder, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0)))
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
                ImGui::TableNextColumn();
                ImGui::ImageButton(entry.path().string().c_str(), (void*)textureIdFile, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));
                ImGui::Text(entry.path().filename().string().c_str());
                count++;
            }
        }

        ImGui::EndTable();
    }

    ImGui::End();
}
