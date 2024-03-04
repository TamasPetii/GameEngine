#pragma once
#include "EngineApi.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <iostream>

class ENGINE_API Camera
{
public:
	Camera();
	~Camera();

	void Update(float deltaTime);
	void Resize(float width, float height);
	void SetView(const glm::vec3& position, const glm::vec3& target, const glm::vec3 up);
	void SetProj(float fov, float width, float height, float near, float far);

	void KeyboardDown(int key);
	void KeyboardUp(int key);
	void MouseMove(int button, int action, int xrel, int yrel);

	inline const auto& GetView()        const { return m_view; }
	inline const auto& GetProj()        const { return m_proj; }
	inline const auto& GetViewInv()     const { return m_viewInv; }
	inline const auto& GetProjInv()     const { return m_projInv; }
	inline const auto& GetViewProj()    const { return m_viewProj; }
	inline const auto& GetViewProjInv() const { return m_viewProjInv; }
	inline const auto& GetWidth()				  const { return m_width; }
	inline const auto& GetHeight()				  const { return m_height; }
	inline const auto& GetFOV()					  const { return m_fov; }
	inline const auto& GetNear()				  const { return m_near; }
	inline const auto& GetFar()					  const { return m_far; }
	inline const auto& GetSensitivity()			  const { return m_sensitivity; }
	inline const auto& GetSpeed()				  const { return m_speed; }
	inline const auto& GetForward()				  const { return m_forward; }
	inline const auto& GetSideways()			  const { return m_sideways; }
	inline const auto& GetPosition() const { return m_position; }
	inline const auto& GetTarget() const { return m_target; }
	inline const auto& GetUp() const { return m_up; }
	inline const auto& GetDirection() const { return m_direction; }
	inline const auto& GetRight() const { return m_right; }
private:
	void UpdateView();
	void UpdateProj();

	glm::mat4 m_view;
	glm::mat4 m_proj;
	glm::mat4 m_viewInv;
	glm::mat4 m_projInv;
	glm::mat4 m_viewProj;
	glm::mat4 m_viewProjInv;
	glm::vec3 m_position;
	glm::vec3 m_target;
	glm::vec3 m_up;
	glm::vec3 m_direction;
	glm::vec3 m_right;
	float m_yaw;
	float m_pitch;
	float m_width;
	float m_height;
	float m_fov;
	float m_near;
	float m_far;
	float m_sensitivity;
	float m_speed;
	float m_forward;
	float m_sideways;
};