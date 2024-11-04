#include "CameraSystem.h"

class gCamera
{
public:
	gCamera(void);
	gCamera(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up);
	~gCamera(void);

	/// <summary>
	/// Gets the view matrix.
	/// </summary>
	/// <returns>The 4x4 view matrix</returns>
	glm::mat4 GetViewMatrix();

	void Update(float _deltaTime);

	void SetView(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up);
	void SetProj(float _angle, float _aspect, float _zn, float _zf);
	void LookAt(glm::vec3 _at);

	void SetSpeed(float _val);
	glm::vec3 GetEye()
	{
		return m_eye;
	}

	glm::vec3 GetAt()
	{
		return m_at;
	}

	glm::vec3 GetUp()
	{
		return m_up;
	}

	glm::mat4 GetProj()
	{
		return m_matProj;
	}

	glm::mat4 GetViewProj()
	{
		return m_matViewProj;
	}

	void Resize(int _w, int _h);

	void KeyboardDown(int key);
	void KeyboardUp(int key);
	void MouseMove(int xrel, int yrel);

	/// <summary>
	/// Updates the UV.
	/// </summary>
	/// <param name="du">The du, i.e. the change of spherical coordinate u.</param>
	/// <param name="dv">The dv, i.e. the change of spherical coordinate v.</param>
	void UpdateUV(float du, float dv);

	/// <summary>
	///  The traversal speed of the camera
	/// </summary>
	float		m_speed;
	/// <summary>
	/// The view matrix of the camera
	/// </summary>
	glm::mat4	m_viewMatrix;

	glm::mat4	m_matViewProj;

	bool	m_slow;

	/// <summary>
	/// The camera position.
	/// </summary>
	glm::vec3	m_eye;

	/// <summary>
	/// The vector pointing upwards
	/// </summary>
	glm::vec3	m_up;

	/// <summary>
	/// The camera look at point.
	/// </summary>
	glm::vec3	m_at;

	/// <summary>
	/// The u spherical coordinate of the spherical coordinate pair (u,v) denoting the
	/// current viewing direction from the view position m_eye. 
	/// </summary>
	float	m_u;

	/// <summary>
	/// The v spherical coordinate of the spherical coordinate pair (u,v) denoting the
	/// current viewing direction from the view position m_eye. 
	/// </summary>
	float	m_v;

	/// <summary>
	/// The distance of the look at point from the camera. 
	/// </summary>
	float	m_dist;

	/// <summary>
	/// The unit vector pointing towards the viewing direction.
	/// </summary>
	glm::vec3	m_fw;
	/// <summary>
	/// The unit vector pointing to the 'right'
	/// </summary>
	glm::vec3	m_st;

	glm::mat4	m_matProj;

	float	m_goFw;
	float	m_goRight;
};

gCamera::gCamera(void) : m_eye(0.0f, 20.0f, 20.0f), m_at(0.0f), m_up(0.0f, 1.0f, 0.0f), m_speed(16.0f), m_goFw(0), m_goRight(0), m_slow(false)
{
	SetView(glm::vec3(0, 20, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	m_dist = glm::length(m_at - m_eye);

	SetProj(glm::radians(60.0f), 640 / 480.0f, 0.01f, 500.f);
}

gCamera::gCamera(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up) : m_speed(16.0f), m_goFw(0), m_goRight(0), m_dist(10), m_slow(false)
{
	SetView(_eye, _at, _up);
}

gCamera::~gCamera(void)
{
}

void gCamera::SetView(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up)
{
	m_eye = _eye;
	m_at = _at;
	m_up = _up;

	m_fw = glm::normalize(m_at - m_eye);
	m_st = glm::normalize(glm::cross(m_fw, m_up));

	m_dist = glm::length(m_at - m_eye);

	m_u = atan2f(m_fw.z, m_fw.x);
	m_v = acosf(m_fw.y);
}

void gCamera::SetProj(float _angle, float _aspect, float _zn, float _zf)
{
	m_matProj = glm::perspective(_angle, _aspect, _zn, _zf);
	m_matViewProj = m_matProj * m_viewMatrix;
}

glm::mat4 gCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

void gCamera::Update(float _deltaTime)
{
	m_eye += (m_goFw * m_fw + m_goRight * m_st) * m_speed * _deltaTime;
	m_at += (m_goFw * m_fw + m_goRight * m_st) * m_speed * _deltaTime;

	m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
	m_matViewProj = m_matProj * m_viewMatrix;
}

void gCamera::UpdateUV(float du, float dv)
{
	m_u += du;
	m_v = glm::clamp<float>(m_v + dv, 0.1f, 3.1f);

	m_at = m_eye + m_dist * glm::vec3(cosf(m_u) * sinf(m_v),
		cosf(m_v),
		sinf(m_u) * sinf(m_v));

	m_fw = glm::normalize(m_at - m_eye);
	m_st = glm::normalize(glm::cross(m_fw, m_up));
}

void gCamera::SetSpeed(float _val)
{
	m_speed = _val;
}

void gCamera::Resize(int _w, int _h)
{
	SetProj(glm::radians(60.0f), _w / (float)_h, 0.01f, 1000.0f);
}

void gCamera::KeyboardDown(int key)
{
	switch (key)
	{
	case GLFW_KEY_W:
		m_goFw = 1;
		break;
	case GLFW_KEY_S:
		m_goFw = -1;
		break;
	case GLFW_KEY_A:
		m_goRight = -1;
		break;
	case GLFW_KEY_D:
		m_goRight = 1;
		break;
	}
}

void gCamera::KeyboardUp(int key)
{
	float current_speed = m_speed;
	switch (key)
	{
	case GLFW_KEY_W:
	case GLFW_KEY_S:
		m_goFw = 0;
		break;
	case GLFW_KEY_A:
	case GLFW_KEY_D:
		m_goRight = 0;
		break;
	}
}

void gCamera::MouseMove(int xrel, int yrel)
{
	UpdateUV(xrel / 100.0f, yrel / 100.0f);
}

void gCamera::LookAt(glm::vec3 _at)
{
	SetView(m_eye, _at, m_up);
}

static gCamera camera;

void CameraSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void CameraSystem::OnUpdate(std::shared_ptr<Registry> registry, float deltaTime)
{
	auto inputManager = InputManager::Instance();
	auto resourceManager = ResourceManager::Instance();
	auto cameraPool = registry->GetComponentPool<CameraComponent>();

	if (!cameraPool)
		return;

	std::for_each(std::execution::seq, cameraPool->GetDenseEntitiesArray().begin(), cameraPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (true || cameraPool->IsFlagSet(entity, UPDATE_FLAG))
			{
				auto& cameraComponent = cameraPool->GetComponent(entity);
				auto index = cameraPool->GetIndex(entity);

				float forward = 0;
				float sideways = 0;

				if (cameraComponent.enableUserMovement)
				{
					if (inputManager->IsKeyHeld(GLFW_KEY_W))
						forward = 1;
					if (inputManager->IsKeyHeld(GLFW_KEY_S))
						forward = -1;
					if (inputManager->IsKeyHeld(GLFW_KEY_D))
						sideways = 1;
					if (inputManager->IsKeyHeld(GLFW_KEY_A))
						sideways = -1;
					if (inputManager->IsButtonHeld(GLFW_MOUSE_BUTTON_RIGHT))
					{
						auto deltaPos = inputManager->GetMouseDelta();
						cameraComponent.yaw += cameraComponent.sensitivity * deltaPos.first;
						cameraComponent.pitch += cameraComponent.sensitivity * -1 * deltaPos.second;
						cameraComponent.pitch = glm::clamp(cameraComponent.pitch, -89.f, 89.f);
					}
				}

				glm::vec3 direction{
							glm::cos(glm::radians(cameraComponent.yaw)) * glm::cos(glm::radians(cameraComponent.pitch)),
							glm::sin(glm::radians(cameraComponent.pitch)),
							glm::sin(glm::radians(cameraComponent.yaw)) * glm::cos(glm::radians(cameraComponent.pitch))
				};


				cameraComponent.direction = glm::normalize(direction);
				cameraComponent.right = glm::normalize(glm::cross(cameraComponent.direction, cameraComponent.up));
				cameraComponent.position += (forward * cameraComponent.direction + sideways * cameraComponent.right) * cameraComponent.speed * deltaTime;
				cameraComponent.target = cameraComponent.position + cameraComponent.direction;
				
				UpdateMatrices(cameraComponent);

				/*
				if (cameraComponent.enableUserMovement)
				{
					if (inputManager->IsKeyHeld(GLFW_KEY_W))
						camera.KeyboardDown(GLFW_KEY_W);
					if (inputManager->IsKeyReleased(GLFW_KEY_W))
						camera.KeyboardUp(GLFW_KEY_W);

					if (inputManager->IsKeyHeld(GLFW_KEY_S))
						camera.KeyboardDown(GLFW_KEY_S);
					if (inputManager->IsKeyReleased(GLFW_KEY_S))
						camera.KeyboardUp(GLFW_KEY_S);

					if (inputManager->IsKeyHeld(GLFW_KEY_D))
						camera.KeyboardDown(GLFW_KEY_D);
					if (inputManager->IsKeyReleased(GLFW_KEY_D))
						camera.KeyboardUp(GLFW_KEY_D);

					if (inputManager->IsKeyHeld(GLFW_KEY_A))
						camera.KeyboardDown(GLFW_KEY_A);
					if (inputManager->IsKeyReleased(GLFW_KEY_A))
						camera.KeyboardUp(GLFW_KEY_A);

					if (inputManager->IsButtonHeld(GLFW_MOUSE_BUTTON_RIGHT))
					{
						auto deltaPos = inputManager->GetMouseDelta();
						camera.MouseMove(deltaPos.first, deltaPos.second);
					}
				}

				static bool init = true;

				if (init)
				{
					init = false;
					camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
					camera.SetProj(glm::radians(60.0f), cameraComponent.width / cameraComponent.height, 0.01f, 500.f);
				}

				camera.Resize(cameraComponent.width, cameraComponent.height);
				camera.Update(deltaTime);

				cameraComponent.view = camera.GetViewMatrix();
				cameraComponent.viewInv = glm::inverse(cameraComponent.view);
				cameraComponent.proj = camera.GetProj();
				cameraComponent.projInv = glm::inverse(cameraComponent.proj);
				cameraComponent.viewProj = camera.GetViewProj();
				cameraComponent.viewProjInv = glm::inverse(cameraComponent.viewProj);
				*/

				if (cameraComponent.isMain)
				{
					UpdateToGpu(cameraComponent);
				}

				cameraPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

}

void CameraSystem::OnEnd(std::shared_ptr<Registry> registry)
{
}

CameraComponent& CameraSystem::GetMainCamera(std::shared_ptr<Registry> registry)
{
	static CameraComponent basicCamera;

	auto cameraPool = registry->GetComponentPool<CameraComponent>();

	if (!cameraPool)
		return basicCamera;

	auto it = std::find_if(cameraPool->GetDenseComponentsArray().begin(), cameraPool->GetDenseComponentsArray().end(),
		[&](const CameraComponent& component) -> bool {
			return component.isMain;
		});

	if (it != cameraPool->GetDenseComponentsArray().end())
		return *it;
	
	return basicCamera;
}

void CameraSystem::UpdateMatrices(CameraComponent& cameraComponent)
{
	cameraComponent.view = glm::lookAt(cameraComponent.position, cameraComponent.target, cameraComponent.up);
	cameraComponent.viewInv = glm::inverse(cameraComponent.view);
	cameraComponent.proj = glm::perspective(glm::radians(cameraComponent.fov), cameraComponent.width / cameraComponent.height, cameraComponent.nearPlane, cameraComponent.farPlane);
	cameraComponent.projInv = glm::inverse(cameraComponent.proj);
	cameraComponent.viewProj = cameraComponent.proj * cameraComponent.view;
	cameraComponent.viewProjInv = glm::inverse(cameraComponent.viewProj);
}

void CameraSystem::UpdateToGpu(CameraComponent& cameraComponent)
{
	auto resourceManager = ResourceManager::Instance();


	static CameraGLSL* camDataSsboHandler = nullptr;
	auto camDataSsbo = resourceManager->GetSsbo("CameraData");
	if (!camDataSsboHandler)
	{
		camDataSsboHandler = static_cast<CameraGLSL*>(camDataSsbo->MapBufferRange());
	}

	auto cameraGLSL = CameraGLSL(cameraComponent);
	camDataSsboHandler->view = cameraGLSL.view;
	camDataSsboHandler->viewInv = cameraGLSL.viewInv;
	camDataSsboHandler->proj = cameraGLSL.proj;
	camDataSsboHandler->projInv = cameraGLSL.projInv;
	camDataSsboHandler->viewProj = cameraGLSL.viewProj;
	camDataSsboHandler->viewProjInv = cameraGLSL.viewProjInv;
	camDataSsboHandler->eye = cameraGLSL.eye;

	//camDataSsbo->UnMapBuffer();
	//camDataSsboHandler = nullptr;
}

void CameraSystem::InvertPitch(CameraComponent& cameraComponent)
{
	cameraComponent.pitch = -cameraComponent.pitch;

	glm::vec3 direction{
		cosf(glm::radians(cameraComponent.yaw)) * cosf(glm::radians(cameraComponent.pitch)),
		sinf(glm::radians(cameraComponent.pitch)),
		sinf(glm::radians(cameraComponent.yaw)) * cosf(glm::radians(cameraComponent.pitch))
	};

	cameraComponent.target = cameraComponent.position + direction;
}

nlohmann::json CameraSystem::Serialize(Registry* registry, Entity entity)
{
	auto& cameraComponent = registry->GetComponent<CameraComponent>(entity);

	nlohmann::json data;
	data["isMain"] = cameraComponent.isMain;
	data["enableUserMovement"] = cameraComponent.enableUserMovement;
	data["nearPlane"] = cameraComponent.nearPlane;
	data["farPlane"] = cameraComponent.farPlane;
	data["yaw"] = cameraComponent.yaw;
	data["pitch"] = cameraComponent.pitch;
	data["fov"] = cameraComponent.fov;
	data["width"] = cameraComponent.width;
	data["height"] = cameraComponent.height;
	data["speed"] = cameraComponent.speed;
	data["sensitivity"] = cameraComponent.sensitivity;

	data["up"]["x"] = cameraComponent.up.x;
	data["up"]["y"] = cameraComponent.up.y;
	data["up"]["z"] = cameraComponent.up.z;

	data["target"]["x"] = cameraComponent.target.x;
	data["target"]["y"] = cameraComponent.target.y;
	data["target"]["z"] = cameraComponent.target.z;

	data["position"]["x"] = cameraComponent.position.x;
	data["position"]["y"] = cameraComponent.position.y;
	data["position"]["z"] = cameraComponent.position.z;

	return data;
}

void CameraSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& cameraComponent = registry->GetComponent<CameraComponent>(entity);

	cameraComponent.isMain = data["isMain"];
	cameraComponent.enableUserMovement = data["enableUserMovement"];
	cameraComponent.nearPlane = data["nearPlane"];
	cameraComponent.farPlane = data["farPlane"];
	cameraComponent.yaw = data["yaw"];
	cameraComponent.pitch = data["pitch"];
	cameraComponent.fov = data["fov"];
	cameraComponent.width = data["width"];
	cameraComponent.height = data["height"];
	cameraComponent.speed = data["speed"];
	cameraComponent.sensitivity = data["sensitivity"];

	cameraComponent.up.x = data["up"]["x"];
	cameraComponent.up.y = data["up"]["y"];
	cameraComponent.up.z = data["up"]["z"];

	cameraComponent.target.x = data["target"]["x"];
	cameraComponent.target.y = data["target"]["y"];
	cameraComponent.target.z = data["target"]["z"];

	cameraComponent.position.x = data["position"]["x"];
	cameraComponent.position.y = data["position"]["y"];
	cameraComponent.position.z = data["position"]["z"];

	registry->SetFlag<CameraComponent>(entity, UPDATE_FLAG);
}