#include "Renderer.h"

void Renderer::RenderScene(std::shared_ptr<Scene> scene)
{
<<<<<<< HEAD
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	auto resourceManager = ResourceManager::Instance();
=======
	if (m_Instance == nullptr)
		m_Instance = new Renderer();

	return m_Instance;
}

void Renderer::Destroy()
{
	if (m_Instance != nullptr)
	{
		delete m_Instance;
		m_Instance = nullptr;
	}
}

Renderer::Renderer()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	m_Camera = std::make_shared<Camera>();
	m_Registry = std::make_shared<Registry>();
	m_ResourceManager = std::make_shared<ResourceManager>();
	m_ResourceManager->Init();
	m_AssetManager = std::make_shared<AssetManager>();

	InitTexture();
	InitRegistry();
}
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

	auto fbo = resourceManager->GetFbo("Main");
	fbo->Clear();
	fbo->Bind();

<<<<<<< HEAD
	ShadowRenderer::Render(scene->GetRegistry());
	GeometryRenderer::Render(scene->GetRegistry());
	DeferredRenderer::Render(scene->GetRegistry());
	BillboardRenderer::Render(scene->GetRegistry());
	WireframeRenderer::Render(scene->GetRegistry());
	SkyboxRenderer::Render(scene->GetRegistry(), scene->GetMainCamera());
	BloomRenderer::Render(scene->GetRegistry());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
=======
}

void Renderer::Update()
{
	static int frames = 0;
	static float time = 0;
	static float lastTime = static_cast<float>(glfwGetTime());
	float currentTime = static_cast<float>(glfwGetTime());
	float deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	time += deltaTime;
	frames += 1;

	if (time > 1)
	{
		std::cout << frames << std::endl;
		time = 0;
		frames = 0;
	}

	m_Camera->Update(deltaTime);

	std::vector<glm::mat4> cameraMatrices{
		m_Camera->GetView(),
		m_Camera->GetViewInv(),
		m_Camera->GetProj(),
		m_Camera->GetProjInv(),
		m_Camera->GetViewProj(),
		m_Camera->GetViewProjInv()
	};

	auto cameraUbo = m_ResourceManager->GetUbo("CameraData");
	cameraUbo->BufferSubStorage(0, 6 * sizeof(glm::mat4), cameraMatrices.data());
	cameraUbo->BufferSubStorage(6 * sizeof(glm::mat4), sizeof(glm::vec4), &m_Camera->GetPosition());

	TransformSystem::OnUpdate(m_Registry, m_ResourceManager);
	MaterialSystem::OnUpdate(m_Registry, m_ResourceManager);
	DirlightSystem::OnUpdate(m_Registry, m_ResourceManager);
	PointLightSystem::OnUpdate(m_Registry, m_ResourceManager);
	SpotLightSystem::OnUpdate(m_Registry, m_ResourceManager);
	DefaultColliderSystem::OnUpdate(m_Registry, m_ResourceManager);
	SphereColliderSystem::OnUpdate(m_Registry, m_ResourceManager);
	MeshColliderSystem::OnUpdate(m_Registry, m_ResourceManager);
	BvhSystem::OnUpdate(m_Registry, m_ResourceManager);
	CollisionSystem::OnUpdate(m_Registry, m_ResourceManager);
	FrustumCullingSystem::OnUpdate(m_Registry, m_ResourceManager);
	InstanceSystem::OnUpdate(m_Registry, m_ResourceManager);

	TransformSystem::OnEnd(m_Registry, m_ResourceManager);
}

void Renderer::Render()
{
	auto start = std::chrono::high_resolution_clock::now();

	{
		auto dirlightPool = m_Registry->GetComponentPool<DirlightComponent>();
		std::for_each(std::execution::seq, dirlightPool->GetDenseEntitiesArray().begin(), dirlightPool->GetDenseEntitiesArray().end(),
			[&](const Entity& entityLight) -> void {
				auto& dirlightComponent = dirlightPool->GetComponent(entityLight);
				auto dirlightIndex = dirlightPool->GetIndex(entityLight);

				if (dirlightComponent.useShadow)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, dirlightComponent.shadowFramebuffer);
					glViewport(0, 0, dirlightComponent.shadowSize, dirlightComponent.shadowSize);
					glClear(GL_DEPTH_BUFFER_BIT);
					m_ResourceManager->GetSsbo("TransformData")->BindBufferBase(0);
					m_ResourceManager->GetSsbo("DirLightData")->BindBufferBase(1);
					auto program = m_ResourceManager->GetProgram("ShadowDir");
					program->Bind();
					program->SetUniform("u_lightIndex", dirlightIndex);

					auto shapePool = m_Registry->GetComponentPool<ShapeComponent>();
					std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
						[&](const Entity& entityShape) -> void {
							if (m_Registry->HasComponent<TransformComponent>(entityShape))
							{
								auto& shapeComponent = shapePool->GetComponent(entityShape);
								auto transformIndex = m_Registry->GetIndex<TransformComponent>(entityShape);
								auto shapeIndex = shapePool->GetIndex(entityShape);

								program->SetUniform("u_transformIndex", transformIndex);
								shapeComponent.shape->Bind();
								glDrawElements(GL_TRIANGLES, shapeComponent.shape->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
								shapeComponent.shape->UnBind();
							}
						}
					);

					program->UnBind();
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}
			}
		);
	}

	{
		auto pointLightPool = m_Registry->GetComponentPool<PointLightComponent>();
		std::for_each(std::execution::seq, pointLightPool->GetDenseEntitiesArray().begin(), pointLightPool->GetDenseEntitiesArray().end(),
			[&](const Entity& entityLight) -> void {
				auto& pointLightComponent = pointLightPool->GetComponent(entityLight);
				auto pointLightIndex = pointLightPool->GetIndex(entityLight);

				if (pointLightComponent.useShadow)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, pointLightComponent.shadowFramebuffer);
					glViewport(0, 0, pointLightComponent.shadowSize, pointLightComponent.shadowSize);
					glClear(GL_DEPTH_BUFFER_BIT);
					m_ResourceManager->GetSsbo("TransformData")->BindBufferBase(0);
					m_ResourceManager->GetSsbo("PointLightData")->BindBufferBase(1);
					auto program = m_ResourceManager->GetProgram("ShadowPoint");
					program->Bind();
					program->SetUniform("u_lightIndex", pointLightIndex);

					auto shapePool = m_Registry->GetComponentPool<ShapeComponent>();
					std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
						[&](const Entity& entityShape) -> void {
							if (m_Registry->HasComponent<TransformComponent>(entityShape))
							{
								auto& shapeComponent = shapePool->GetComponent(entityShape);
								auto transformIndex = m_Registry->GetIndex<TransformComponent>(entityShape);
								auto shapeIndex = shapePool->GetIndex(entityShape);

								program->SetUniform("u_transformIndex", transformIndex);
								shapeComponent.shape->Bind();
								glDrawElements(GL_TRIANGLES, shapeComponent.shape->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
								shapeComponent.shape->UnBind();
							}

						}
					);

					program->UnBind();
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}
			}
		);
	}

	auto fbo = m_ResourceManager->GetFbo("Main");
	fbo->Clear();
	fbo->Bind(GL_FRAMEBUFFER);

	{
		fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT5 });
		m_ResourceManager->GetUbo("CameraData")->BindBufferBase(0);
		m_ResourceManager->GetSsbo("TransformData")->BindBufferBase(1);
		m_ResourceManager->GetSsbo("MaterialData")->BindBufferBase(2);
		auto program = m_ResourceManager->GetProgram("DeferredPre");
		program->Bind();
		auto shapePool = m_Registry->GetComponentPool<ShapeComponent>();
		std::for_each(std::execution::seq, shapePool->GetDenseEntitiesArray().begin(), shapePool->GetDenseEntitiesArray().end(),
			[&](const Entity& entity) -> void {
				if (m_Registry->HasComponent<TransformComponent>(entity) && m_Registry->HasComponent<MaterialComponent>(entity))
				{
					auto& shapeComponent = shapePool->GetComponent(entity);
					auto transformIndex = m_Registry->GetIndex<TransformComponent>(entity);
					auto materialIndex = m_Registry->GetIndex<MaterialComponent>(entity);
					auto entityIndex = entity;

					if (shapeComponent.toRender && !shapeComponent.isInstanced)
					{
						program->SetUniform("u_transformIndex", transformIndex);
						program->SetUniform("u_materialIndex", materialIndex);
						program->SetUniform("u_entityIndex", entityIndex);
						shapeComponent.shape->Bind();
						glDrawElements(GL_TRIANGLES, shapeComponent.shape->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
						shapeComponent.shape->UnBind();
					}
				}
			}
		);
		program->UnBind();
	}

	{
		fbo->ActivateTextures(std::vector<GLenum>{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT5 });
		m_ResourceManager->GetUbo("CameraData")->BindBufferBase(0);
		m_ResourceManager->GetGeometry("Cube")->GetInstanceSsbo()->BindBufferBase(1);
		m_ResourceManager->GetSsbo("TransformData")->BindBufferBase(2);
		m_ResourceManager->GetSsbo("MaterialData")->BindBufferBase(3);
		auto program = m_ResourceManager->GetProgram("DeferredPreInstance");
		program->Bind();
		m_ResourceManager->GetGeometry("Cube")->Bind();
		glDrawElementsInstanced(GL_TRIANGLES, m_ResourceManager->GetGeometry("Cube")->GetIndexCount(), GL_UNSIGNED_INT, nullptr, m_ResourceManager->GetGeometry("Cube")->GetInstances().size());
		m_ResourceManager->GetGeometry("Cube")->UnBind();
		program->UnBind();
	}


	DeferredRenderer::Render(m_Registry, m_ResourceManager);
	BillboardRenderer::Render(m_Registry, m_ResourceManager, m_AssetManager);
	WireframeRenderer::Render(m_Registry, m_ResourceManager);

	{//Skybox
		GLint prevDepthFnc;
		glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);
		glDepthFunc(GL_LEQUAL);
		auto program = m_ResourceManager->GetProgram("Skybox");
		program->Bind();
		program->SetUniform("model", glm::translate(m_Camera->GetPosition()) * glm::scale(glm::vec3(-1)));
		program->SetTexture("skyboxTexture", 0, m_AssetManager->GetImageTexture("../Assets/Skybox.png")->GetTextureID());
		m_ResourceManager->GetGeometry("Cube")->Bind();
		glDrawElements(GL_TRIANGLES, m_ResourceManager->GetGeometry("Cube")->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
		m_ResourceManager->GetGeometry("Cube")->UnBind();
		program->UnBind();
		glDepthFunc(prevDepthFnc);
	}

	BloomRenderer::Render(m_Registry, m_ResourceManager);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::InitTexture()
{
	m_AssetManager->LoadImageTexture("../Assets/Diffuse.jpg");
	m_AssetManager->LoadImageTexture("../Assets/Specular.jpg");
	m_AssetManager->LoadImageTexture("../Assets/Normal.jpg");
	m_AssetManager->LoadImageTexture("../Assets/PointLightIcon.png");
	m_AssetManager->LoadImageTexture("../Assets/DirLightIcon.png");
	m_AssetManager->LoadImageTexture("../Assets/SpotLightIcon.png");
	m_AssetManager->LoadImageTextureMap("../Assets/Skybox.png");
}

void Renderer::InitRegistry()
{
	std::random_device rnd;
	std::mt19937_64 gen(rnd());
	std::uniform_real_distribution<float> dist(-1, 1);

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<MeshCollider>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->GetComponent<ShapeComponent>(entity).shape = m_ResourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(500, 0.1, 500);
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<SphereCollider>(entity);
		m_Registry->GetComponent<ShapeComponent>(entity).shape = m_ResourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(0, 5.2, 0);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(5, 5, 5);
		m_Registry->GetComponent<MaterialComponent>(entity).color = glm::vec4(1, 1, 0, 1);
		m_Registry->GetComponent<MaterialComponent>(entity).diffuse = m_AssetManager->GetImageTexture("../Assets/Diffuse.jpg");
		m_Registry->GetComponent<MaterialComponent>(entity).specular = m_AssetManager->GetImageTexture("../Assets/Specular.jpg");
		m_Registry->GetComponent<MaterialComponent>(entity).normal = m_AssetManager->GetImageTexture("../Assets/Normal.jpg");
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<DirlightComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		auto pool = m_Registry->GetComponentPool<DirlightComponent>();
		pool->SetFlag(entity, REGENERATE_FLAG);
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<PointLightComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		auto pool = m_Registry->GetComponentPool<PointLightComponent>();
		pool->SetFlag(entity, REGENERATE_FLAG);
	}

	for (int i = 0; i < 100; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<DefaultCollider>(entity);
		m_Registry->GetComponent<ShapeComponent>(entity).isInstanced = true;
		m_Registry->GetComponent<ShapeComponent>(entity).shape = m_ResourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(dist(gen) * 100, (dist(gen) + 1) * 100, dist(gen) * 100);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(1);
		m_Registry->GetComponent<MaterialComponent>(entity).color = glm::vec4((dist(gen) + 1) * 0.5, (dist(gen) + 1) * 0.5, (dist(gen) + 1) * 0.5, 1);
		m_Registry->GetComponent<MaterialComponent>(entity).useBloom = true;
	}

	for (int i = 0; i < 5; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<PointLightComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3((i / 8) * 5, 1, (i % 8) * 5);
		m_Registry->GetComponent<PointLightComponent>(entity).color = glm::vec3((dist(gen) + 1) *0.5, (dist(gen) + 1) * 0.5, (dist(gen) + 1) * 0.5);
		m_Registry->SetFlag<PointLightComponent>(entity, REGENERATE_FLAG);
	}

	for (int i = 0; i < 5; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<SpotLightComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(-(i / 8) * 5, 1, -(i % 8) * 5);
		m_Registry->GetComponent<SpotLightComponent>(entity).color = glm::vec3((dist(gen) + 1) * 0.5, (dist(gen) + 1) * 0.5, (dist(gen) + 1) * 0.5);
		m_Registry->SetFlag<SpotLightComponent>(entity, REGENERATE_FLAG);
	}
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
}