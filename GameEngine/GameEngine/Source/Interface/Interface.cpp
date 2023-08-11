#include "Interface.h"

Interface::Interface(GLFWwindow* window, Renderer* renderer)
{
	mWindow = window;
	mRenderer = renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
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
    Interface::RenderEntityWindow();
    Interface::RenderComponentsWindow();
    Interface::RenderSettingsWindow();
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
    static ImGuizmo::MODE currentMode = ImGuizmo::LOCAL;
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
        glm::vec3 translation, rotation, scale;
        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(cubeTransform), glm::value_ptr(translation), glm::value_ptr(rotation), glm::value_ptr(scale));
        
        switch (currentOperation)
        {
        case ImGuizmo::TRANSLATE:
            transform->GetTranslation() = translation;
            break;
        case ImGuizmo::ROTATE:
            std::cout << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;
            glm::vec3 rotationDelta = rotation - transform->GetRotation();
            transform->GetRotation() += rotationDelta;
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

void Interface::RenderEntityWindow()
{
    ImGui::ShowDemoWindow();

    ImGui::Begin("entitys");

    for (auto entity : mRenderer->GetEntities())
    {
        std::string headerText = entity->GetText() + std::to_string(entity->GetId());
        if (ImGui::CollapsingHeader(headerText.c_str()))
        { 
        }
    }

    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Selectable("Empty"))
        {
            Entity* entity = new Entity;
            entity->AddComponent(new TransformComponent());
            mRenderer->GetEntities().insert(entity);
        }
        if (ImGui::BeginMenu("Shapes"))
        {
            if (ImGui::Selectable("Cube"))
            {
                Entity* entity = new Entity;
                entity->AddComponent(new TransformComponent());
                MeshComponent* mesh = new MeshComponent();
                mesh->AttachMesh(new Shape<Cube>());
                entity->AddComponent(mesh);
                mRenderer->GetEntities().insert(entity);
            }
            if (ImGui::Selectable("Cylinder"))
            {
                Entity* entity = new Entity;
                entity->AddComponent(new TransformComponent());
                MeshComponent* mesh = new MeshComponent();
                mesh->AttachMesh(new Shape<Cylinder>());
                entity->AddComponent(mesh);
                mRenderer->GetEntities().insert(entity);
            }
            if (ImGui::Selectable("Sphere"))
            {
                Entity* entity = new Entity;
                entity->AddComponent(new TransformComponent());
                MeshComponent* mesh = new MeshComponent();
                mesh->AttachMesh(new Shape<Sphere>());
                entity->AddComponent(mesh);
                mRenderer->GetEntities().insert(entity);
            }
            if (ImGui::Selectable("Torus"))
            {
                Entity* entity = new Entity;
                entity->AddComponent(new TransformComponent());
                MeshComponent* mesh = new MeshComponent();
                mesh->AttachMesh(new Shape<Torus>());
                entity->AddComponent(mesh);
                mRenderer->GetEntities().insert(entity);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Lights"))
        {
            if (ImGui::Selectable("Direction"))
            {
                Entity* entity = new Entity;
                entity->AddComponent(new TransformComponent());
                LightComponent* light = new LightComponent();
                light->AttachLight(new Light<Directional>());
                entity->AddComponent(light);
                mRenderer->GetEntities().insert(entity);
            }
            if (ImGui::Selectable("Point"))
            {
                Entity* entity = new Entity;
                entity->AddComponent(new TransformComponent());
                LightComponent* light = new LightComponent();
                light->AttachLight(new Light<Point>());
                entity->AddComponent(light);
                mRenderer->GetEntities().insert(entity);
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
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
                if(item_current != 0)
                    mesh->SetTexture(Texture2D::LoadTexture2D(items[item_current]));
                else
                    mesh->SetTexture(nullptr);
            }

            std::string colorLabel = "##Color" + std::to_string(entity->GetId());
            ImGui::Text("Scale");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            ImGui::ColorEdit3(colorLabel.c_str(), &mesh->GetColor().x, 0.05f);
        }

        if (entity->HasComponent<LightComponent>() && ImGui::CollapsingHeader("Light"))
        {
            LightComponent* transform = entity->GetComponent<LightComponent>();
        }
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

    /*

    ImGui::Image((void*)mRenderer->GetShadowFrameBuffer()->GetTextureId(), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::CollapsingHeader("ShadowMap"))
    {
        for (entity* entity : mRenderer->Getentitys())
        {
                if (dynamic_cast<DirectionLight*>(entity))
                {
                    ImGui::SeparatorText("Advanced");

                    DirectionLight* light = dynamic_cast<DirectionLight*>(entity);

                    ImGui::Text("Position");
                    ImGui::SameLine();
                    ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    ImGui::DragFloat3("##LightPosition", &light->mPosition[0], 0.05f);

                    ImGui::Text("Direction");
                    ImGui::SameLine();
                    ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    ImGui::DragFloat3("##LightDirection", &light->mDirection[0], 0.05f);

                    ImGui::Text("ProjX");
                    ImGui::SameLine();
                    ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    ImGui::DragFloat2("##LightProjX", &light->mProjX[0], 0.05f);

                    ImGui::Text("ProjY");
                    ImGui::SameLine();
                    ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    ImGui::DragFloat2("##LightProjY", &light->mProjY[0], 0.05f);

                    ImGui::Text("ProjZ");
                    ImGui::SameLine();
                    ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                    ImGui::DragFloat2("##LightProjZ", &light->mProjZ[0], 0.05f);

                }
        }
    }
    */

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