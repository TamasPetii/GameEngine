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
        mRenderer->GetSceneFrameBuffer()->Resize(mViewPortSize.x, mViewPortSize.y);
        mRenderer->GetItemPickFrameBuffer()->Resize(mViewPortSize.x, mViewPortSize.y);
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
    Interface::RenderGameObjectWindow();
    Interface::RenderComponentsWindow();
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

    if (size.x != mRenderer->GetSceneFrameBuffer()->GetWidth() || size.y != mRenderer->GetSceneFrameBuffer()->GetHeight())
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
            int id = mRenderer->GetItemPickFrameBuffer()->ReadPixel(mouseX, mouseY);
            std::cout << id << std::endl;

            if (!mRenderer->FindActiveObject(id) && !ImGuizmo::IsUsing())
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
    if (mRenderer->GetActiveObject() == nullptr) return;

    static ImGuizmo::OPERATION currentOperation = ImGuizmo::TRANSLATE;
    static ImGuizmo::MODE currentMode = ImGuizmo::LOCAL;

    glm::mat4 viewMatrix = mRenderer->GetCamera()->GetViewMatrix();
    glm::mat4 projectionMatrix = mRenderer->GetCamera()->GetProjMatrix();
    glm::mat4 cubeTransform = mRenderer->GetActiveObject()->GetTransformMatrix();

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
            mRenderer->GetActiveObject()->GetTranslateRef() = translation;
            break;
        case ImGuizmo::ROTATE:
            mRenderer->GetActiveObject()->GetRotationRef() = rotation;
            break;
        case ImGuizmo::SCALE:
            mRenderer->GetActiveObject()->GetScaleRef() = scale;
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

void Interface::RenderGameObjectWindow()
{
    ImGui::ShowDemoWindow();

    ImGui::Begin("GameObjects");

    std::cout << mRenderer->GetGameObjects().size() << std::endl;

    for (GameObject* gameObject : mRenderer->GetGameObjects())
    {
        std::string headerText = gameObject->GetNameRef() + std::to_string(gameObject->GetId());
        if (ImGui::CollapsingHeader(headerText.c_str()))
        { 
        }
    }

    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Selectable("New Cube"))
        {
            mRenderer->GetGameObjects().insert(new Cube());
        }
        ImGui::EndPopup();
    }


    ImGui::End();
}

void Interface::RenderComponentsWindow()
{
    ImGui::Begin("Components");

    GameObject* gameObject = mRenderer->GetActiveObject();
    if (gameObject && ImGui::CollapsingHeader("Transformation"))
    {

        std::string translateLabel = "##Translate" + std::to_string(gameObject->GetId());
        ImGui::Text("Translation");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::DragFloat3(translateLabel.c_str(), &gameObject->GetTranslateRef().x, 0.05f);

        std::string rotationLabel = "##Rotate" + std::to_string(gameObject->GetId());
        ImGui::Text("Rotation");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::DragFloat3(rotationLabel.c_str(), &gameObject->GetRotationRef().x, 0.05f);

        std::string scaleLabel = "##Scale" + std::to_string(gameObject->GetId());
        ImGui::Text("Scale");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::DragFloat3(scaleLabel.c_str(), &gameObject->GetScaleRef().x, 0.05f);

        std::string angleLabel = "##Angle" + std::to_string(gameObject->GetId());
        ImGui::Text("Angle");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::DragFloat(angleLabel.c_str(), &gameObject->GetAngleRef(), 0.05f);
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