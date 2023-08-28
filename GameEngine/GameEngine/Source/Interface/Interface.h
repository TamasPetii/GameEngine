#pragma once
#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>
#include <IMGUI/ImGuizmo.h>
#include <IMGUI/imgui_internal.h>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/detail/type_quat.hpp>
#include "../Render/Renderer.h"
#include <filesystem>
#include <any>


enum MessageType
{
	TEXTURE
};

struct Message
{
	MessageType type;
	const void* data;
	int size;
};

class Interface
{
public:
	Interface(GLFWwindow* window, Renderer* renderer);
	void Render();
	void Update();
private:
	void PreRender();
	void PostRender();
	void RenderDockSpace();
	void RenderViewPortWindow();
	void RenderGizmos();
	void RenderComponentsWindow();
	void RenderSettingsWindow();
	void Camera_KeyboardEvent();
	void Camera_MouseEvent();
	void EntityWindow();
	void FileSystemWindow();
	void DisplayEntity(Entity* entity);

	GLFWwindow* mWindow;
	Renderer* mRenderer;
	ImVec2 mViewPortClickCoords;
	ImVec2 mViewPortSize;
	bool mViewPortResize;


	ImageTexture* pauseButtonImage;
	ImageTexture* playButtonImage;
	ImageTexture* stopButtonImage;
	ImageTexture* fileImage;
	ImageTexture* folderImage;
	ImageTexture* parentImage;
};

