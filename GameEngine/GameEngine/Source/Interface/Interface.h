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
#include "../Renderer/Renderer.h"
#include <filesystem>
#include <any>
#include <functional>

enum MessageType
{
	TEXTURE,
	OBJECT
};

struct Message
{
	MessageType type;
	const void* data;
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

	static Renderer* mRenderer;
	GLFWwindow* mWindow;
	ImVec2 mViewPortClickCoords;
	ImVec2 mViewPortSize;
	bool mViewPortResize;
	Entity* scriptEntity;

	ImageTexture* pauseButtonImage;
	ImageTexture* playButtonImage;
	ImageTexture* stopButtonImage;
	ImageTexture* fileImage;
	ImageTexture* folderImage;
	ImageTexture* parentImage;

	//void BindDropTarget(const std::string& acceptText, );

	static void DrawMeshComponentUI(MeshComponent* meshComponent);
	static void DrawLightComponentUI(LightComponent* lightComponent);
	static void DrawTransformComponentUI(TransformComponent* transformComponent);
	static void DrawScriptComponentUI(ScriptComponent* scriptComponent);

	static std::string label;
	static void DrawColorEdit3(const std::string& text, glm::vec3& color);
	static void DrawButton(const std::string& text, const ImVec2& size = ImVec2(0, 0), std::function<void()> callback = []() -> void{});

	static void AttachDropTarget(const std::string& acceptText, std::function<void(const void* data)> callback);
	static void AcceptDroppedDiffuseTexture(const void* data);
	static void AcceptDroppedNormalTexture(const void* data);
	static void AcceptDroppedHeightTexture(const void* data);

	static void asdasd();

	static void DrawLeftLabel(const std::string& text, int width = 100);
	static void DrawDragFloat(const std::string& text, float& value, float speed = 0.05f);

};
