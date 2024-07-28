#include "Camera.h"

Camera::Camera() :
	m_view{ glm::mat4(1.f) },
	m_proj{ glm::mat4(1.f) },
	m_viewInv{ glm::mat4(1.f) },
	m_projInv{ glm::mat4(1.f) },
	m_viewProj{ glm::mat4(1.f) },
	m_viewProjInv{ glm::mat4(1.f) },
	m_position{ glm::vec3(0.f,0.f,5.f) }, m_target{ glm::vec3(0.f,0.f,0.f) }, m_up{ glm::vec3(0.f,1.f,0.f) },
	m_direction{ glm::vec3(0.f,0.f,-1.f) }, m_right{ glm::vec3(1.f, 0.f, 0.f) },
	m_width{ 1024.f }, m_height{ 768.f }, m_fov{ 60.f }, m_near{ 0.01f }, m_far{ 500.f },
	m_sensitivity{ 1.f }, m_speed{ 25.f }, m_forward{ 0.f }, m_sideways{ 0.f },
	m_yaw{ 0.f }, m_pitch{ 0.f }
{
	UpdateView();
	UpdateProj();

	m_yaw = glm::degrees(atan2f(m_direction.z, m_direction.x));
	m_pitch = glm::degrees(asinf(m_direction.y));
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	m_position += (m_forward * m_direction + m_sideways * m_right) * m_speed * deltaTime;
	m_target += (m_forward * m_direction + m_sideways * m_right) * m_speed * deltaTime;
	UpdateView();
}

#include <Manager/ResourceManager.h>
//THIS SHOULD BE ON CAMERA SYSTEM

void Camera::UpdateGLSL()
{
	auto resourceManager = ResourceManager::Instance();

	std::vector<glm::mat4> cameraMatrices{
		this->GetView(),
		this->GetViewInv(),
		this->GetProj(),
		this->GetProjInv(),
		this->GetViewProj(),
		this->GetViewProjInv()
	};

	auto cameraUbo = resourceManager->GetUbo("CameraData");
	cameraUbo->BufferSubStorage(0, 6 * sizeof(glm::mat4), cameraMatrices.data());
	cameraUbo->BufferSubStorage(6 * sizeof(glm::mat4), sizeof(glm::vec4), &this->GetPosition());
}

void Camera::KeyboardDown(int key)
{
	
	switch (key)
	{
	case GLFW_KEY_W:
		m_forward = 1;
		break;
	case GLFW_KEY_S:
		m_forward = -1;
		break;
	case GLFW_KEY_A:
		m_sideways = -1;
		break;
	case GLFW_KEY_D:
		m_sideways = 1;
		break;
	}
}

void Camera::KeyboardUp(int key)
{
	switch (key)
	{
	case GLFW_KEY_W:
	case GLFW_KEY_S:
		m_forward = 0;
		break;
	case GLFW_KEY_A:
	case GLFW_KEY_D:
		m_sideways = 0;
		break;
	}
}

void Camera::MouseMove(int button, int action, int xrel, int yrel)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		m_yaw += m_sensitivity * xrel;
		m_pitch += m_sensitivity * -1 * yrel;
		m_pitch = glm::clamp(m_pitch, -89.f, 89.f);

		glm::vec3 direction{
			cosf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch)),
			sinf(glm::radians(m_pitch)),
			sinf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch))
		};

		m_target = m_position + direction;
		m_direction = glm::normalize(m_target - m_position);
		m_right = glm::normalize(glm::cross(m_direction, m_up));
	}
}

void Camera::SetView(const glm::vec3& position, const glm::vec3& target, const glm::vec3 up)
{
	m_position = position;
	m_target = target;
	m_up = up;
	UpdateView();
}

void Camera::UpdateView()
{
	m_direction = glm::normalize(m_target - m_position);
	m_right = glm::normalize(glm::cross(m_direction, m_up));
	m_view = glm::lookAt(m_position, m_target, m_up);
	m_viewInv = glm::inverse(m_view);
	m_viewProj = m_proj * m_view;
	m_viewProjInv = glm::inverse(m_viewProj);
}

void Camera::UpdateProj()
{
	m_sensitivity = 0.5f * m_fov / 60.f;
	m_proj = glm::perspective(glm::radians(m_fov), m_width / m_height, m_near, m_far);
	m_projInv = glm::inverse(m_proj);
	m_viewProj = m_proj * m_view;
	m_viewProjInv = glm::inverse(m_viewProj);
}

void Camera::InvertPitch()
{
	m_pitch = -m_pitch;

	glm::vec3 direction{
		cosf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch)),
		sinf(glm::radians(m_pitch)),
		sinf(glm::radians(m_yaw)) * cosf(glm::radians(m_pitch))
	};

	m_target = m_position + direction;
	m_direction = glm::normalize(m_target - m_position);
	m_right = glm::normalize(glm::cross(m_direction, m_up));
}

void Camera::SetProj(float fov, float width, float height, float near, float far)
{
	m_fov = fov;
	m_width = width;
	m_height = height;
	m_near = near;
	m_far = far;
	UpdateProj();
}

void Camera::Resize(float width, float height)
{
	SetProj(m_fov, width, height, m_near, m_far);
}