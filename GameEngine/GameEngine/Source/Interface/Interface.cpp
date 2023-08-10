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
    Interface::RenderGameObjectWindow();
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

            if (!ImGuizmo::IsUsing() && !ImGuizmo::IsOver() && ImGui::IsWindowHovered() && !mRenderer->FindActiveObject(id))
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
            std::cout << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;
            glm::vec3 rotationDelta = rotation - mRenderer->GetActiveObject()->GetRotationRef();
            mRenderer->GetActiveObject()->GetRotationRef() += rotationDelta;
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

    for (GameObject* gameObject : mRenderer->GetGameObjects())
    {
        std::string headerText = gameObject->GetNameRef() + std::to_string(gameObject->GetId());
        if (ImGui::CollapsingHeader(headerText.c_str()))
        { 
        }
    }

    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Selectable("Empty"))
        {
            mRenderer->GetGameObjects().insert(new Cube());
        }
        if (ImGui::BeginMenu("Shapes"))
        {
            if (ImGui::Selectable("Plane"))
            {
                mRenderer->GetGameObjects().insert(new Plane());
            }
            if (ImGui::Selectable("Cube"))
            {
                mRenderer->GetGameObjects().insert(new Cube());
            }
            if (ImGui::Selectable("Cylinder"))
            {
                mRenderer->GetGameObjects().insert(new Cylinder());
            }
            if (ImGui::Selectable("Sphere"))
            {
                mRenderer->GetGameObjects().insert(new Sphere());
            }
            if (ImGui::Selectable("Torus"))
            {
                mRenderer->GetGameObjects().insert(new Torus());
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Lights"))
        {
            if (ImGui::Selectable("Direction"))
            {
                mRenderer->GetGameObjects().insert(new DirectionLight());
            }
            if (ImGui::Selectable("Point"))
            {
                mRenderer->GetGameObjects().insert(new PointLight());
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

    GameObject* gameObject = mRenderer->GetActiveObject();

    if (dynamic_cast<Shape*>(gameObject) && ImGui::CollapsingHeader("Details"))
    {
        Shape* shape = dynamic_cast<Shape*>(gameObject);
        ImGui::Text("Vertices: %d", shape->GetVertexCount());
        ImGui::Text("Indices: %d", shape->GetIndexCount());
    }

    if (dynamic_cast<Shape*>(gameObject) && ImGui::CollapsingHeader("Properties"))
    {
        Shape* shape = dynamic_cast<Shape*>(gameObject);  

        ImGui::SeparatorText("General");

        ImGui::Text("Color");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::ColorEdit3("##Color", &shape->GetColorRef()[0]);
        
        /*
        std::vector<std::string> itemsStr = Texture2D::GetTextureNames();
        //int item_current = shape->GetTexture() == nullptr ? -1 : ;
        static int item_current = -1;
        ImGui::Text("Texture");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        if (ImGui::Combo("##Texture", &item_current, itemsStr.data()->c_str(), IM_ARRAYSIZE(itemsStr.data()->c_str())) && item_current != -1)
        {
            Texture2D* textureComponent = shape->GetTexture();     
            *textureComponent = *Texture2D::LoadTexture2D(itemsStr[item_current]);
        }

        ImGui::Text("Use Tex");
        ImGui::SameLine();
        ImGui::Checkbox("##UseTexture", &shape->GetUseTextureRef());
        */

        if (dynamic_cast<Cylinder*>(gameObject))
        {
            ImGui::SeparatorText("Advanced");

            Cylinder* cylinder = dynamic_cast<Cylinder*>(gameObject);

            std::string textLabel = "##RadiusT";
            ImGui::Text("RadiusT");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::DragFloat(textLabel.c_str(), &cylinder->GetRadiusTopRef(), 0.05f))
                cylinder->UpdateVertices();

            textLabel = "##RadiusB";
            ImGui::Text("RadiusB");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::DragFloat(textLabel.c_str(), &cylinder->GetRadiusBottomRef(), 0.05f))
                cylinder->UpdateVertices();

            textLabel = "##Points";
            ImGui::Text("Points");
            ImGui::SameLine();
            ImGui::SetCursorPos(ImVec2(90, ImGui::GetCursorPos().y));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
            if (ImGui::DragInt(textLabel.c_str(), &cylinder->GetPointsRef(), 0.05f, 3))
            {
                if (cylinder->GetPointsRef() < 3) cylinder->GetPointsRef() = 3;
                cylinder->UpdateBuffers();
            }
                
        }

        
    }

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

    ImGui::Image((void*)mRenderer->GetShadowFrameBuffer()->GetTextureId(), ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::CollapsingHeader("ShadowMap"))
    {
        for (GameObject* gameObject : mRenderer->GetGameObjects())
        {
                if (dynamic_cast<DirectionLight*>(gameObject))
                {
                    ImGui::SeparatorText("Advanced");

                    DirectionLight* light = dynamic_cast<DirectionLight*>(gameObject);

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