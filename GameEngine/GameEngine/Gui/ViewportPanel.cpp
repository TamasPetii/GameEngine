#include "ViewportPanel.h"

void ViewportPanel::Update()
{
    if (m_ViewportSizeChanged)
    {
        m_ViewportSizeChanged = false;
        Renderer::Instance()->m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
        Renderer::Instance()->m_Camera->Resize(m_ViewportSize.x, m_ViewportSize.y);
    }
}

void ViewportPanel::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	if (ImGui::Begin(TITLE_VP("ViewPort")))
	{
        ImVec2 size = ImGui::GetContentRegionAvail();
        auto& fbo = Renderer::Instance()->m_Framebuffer;

        ImGui::Image((ImTextureID)fbo->GetTextureID("color"), size, ImVec2(0, 1), ImVec2(1, 0));

        if (size.x != fbo->GetSize().x || size.y != fbo->GetSize().y)
        {
            m_ViewportSize = size;
            m_ViewportSizeChanged = true;
        }

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
                ImGui::IsWindowHovered())
            {
                unsigned int id = std::any_cast<unsigned int>(fbo->ReadPixel("id", mouseX, mouseY));
                std::cout << "Instance ID: " << id << std::endl;
            }
        }

        ViewportPanel::CameraKeyboardEvent(Renderer::Instance()->m_Camera);
        ViewportPanel::CameraButtonEvent(Renderer::Instance()->m_Camera);
	}
    ImGui::PopStyleVar();

	ImGui::End();
}

void ViewportPanel::CameraKeyboardEvent(std::shared_ptr<Camera> camera)
{
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
}

void ViewportPanel::CameraButtonEvent(std::shared_ptr<Camera> camera)
{
    static bool isMouseClicked = false;

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        isMouseClicked = true;
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
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