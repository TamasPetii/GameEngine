#include "Scene.h"

using namespace physx;

Scene::Scene()
{
	auto textureManager = TextureManager::Instance();
	auto resourceManager = ResourceManager::Instance();

	m_Registry = std::make_shared<Registry>();
	m_SceneCamera = std::make_shared<Camera>();

	std::random_device rnd;
	std::mt19937_64 gen(rnd());
	std::uniform_real_distribution<float> dist(-1, 1);

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -9.81f, 0);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	{
		auto entity = m_Registry->CreateEntity();

		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "Light";
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<DirlightComponent>(entity);
		m_Registry->GetComponent<DirlightComponent>(entity).strength = 1;
		m_Registry->GetComponent<DirlightComponent>(entity).useShadow = true;
		m_Registry->SetFlag<DirlightComponent>(entity, REGENERATE_FLAG);
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "Floor";
		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(100, 1, 100);

		m_Registry->AddComponent<BoxColliderComponent>(entity);
		m_Registry->AddComponent<RigidbodyStaticComponent>(entity);
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "WallLeft";
		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(-100, 50, 0);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(1, 50, 100);

		m_Registry->AddComponent<BoxColliderComponent>(entity);
		m_Registry->AddComponent<RigidbodyStaticComponent>(entity);
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "WallRight";
		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(100, 50, 0);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(1, 50, 100);

		m_Registry->AddComponent<BoxColliderComponent>(entity);
		m_Registry->AddComponent<RigidbodyStaticComponent>(entity);
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "WallBack";
		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(1, 50, -100);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(100, 50, 1);

		m_Registry->AddComponent<BoxColliderComponent>(entity);
		m_Registry->AddComponent<RigidbodyStaticComponent>(entity);
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "WallFront";
		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(1, 50, 100);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(100, 50, 1);

		m_Registry->AddComponent<BoxColliderComponent>(entity);
		m_Registry->AddComponent<RigidbodyStaticComponent>(entity);
	}

	for (int i = 0; i < 100; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->AddComponent<BoxColliderComponent>(entity);
		m_Registry->AddComponent<RigidbodyDynamicComponent>(entity);

		m_Registry->GetComponent<TagComponent>(entity).name = "Cube";
		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(dist(gen) * 90, dist(gen) * 25 + 100, dist(gen) * 90);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(dist(gen) + 1.5);
		m_Registry->GetComponent<MaterialComponent>(entity).color = glm::vec4(dist(gen) * 0.5 + 0.5, dist(gen) * 0.5 + 0.5, dist(gen) * 0.5 + 0.5, 1);
	}

	for (int i = 0; i < 100; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "Sphere";
		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Sphere");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(dist(gen) * 90, dist(gen) * 25 + 100, dist(gen) * 90);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(dist(gen) + 1.5);
		m_Registry->GetComponent<MaterialComponent>(entity).color = glm::vec4(dist(gen) * 0.5 + 0.5, dist(gen) * 0.5 + 0.5, dist(gen) * 0.5 + 0.5, 1);

		m_Registry->AddComponent<SphereColliderComponent>(entity);
		m_Registry->AddComponent<RigidbodyDynamicComponent>(entity);
	}

	for (int i = 0; i < 100; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "Cone";
		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cone");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(dist(gen) * 90, dist(gen) * 25 + 100, dist(gen) * 90);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(dist(gen) + 1.5);
		m_Registry->GetComponent<MaterialComponent>(entity).color = glm::vec4(dist(gen) * 0.5 + 0.5, dist(gen) * 0.5 + 0.5, dist(gen) * 0.5 + 0.5, 1);
		
		m_Registry->AddComponent<ConvexColliderComponent>(entity);
		m_Registry->AddComponent<RigidbodyDynamicComponent>(entity);
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<ModelComponent>(entity);

		m_Registry->GetComponent<TagComponent>(entity).name = "Model";
		m_Registry->GetComponent<ModelComponent>(entity).model = ModelManager::Instance()->LoadModel("C:/Users/User/Desktop/GameEngine/GameEngine/Assets/Models/Mamut.obj");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(0, 15, 0);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(2);

		m_Registry->AddComponent<MeshColliderComponent>(entity);
		m_Registry->AddComponent<RigidbodyStaticComponent>(entity);
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<ModelComponent>(entity);

		m_Registry->GetComponent<TagComponent>(entity).name = "Monster";
		m_Registry->GetComponent<ModelComponent>(entity).model = ModelManager::Instance()->LoadModel("C:/Users/User/Desktop/GameEngine/GameEngine/Assets/Models/Mixamo/Monster - Falling/Falling.dae");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(0, 15, 0);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(0.1, 0.1, 0.1);

		m_Registry->AddComponent<MeshColliderComponent>(entity);
		m_Registry->AddComponent<RigidbodyStaticComponent>(entity);

		ModelManager::Instance()->LoadAnimation("C:/Users/User/Desktop/GameEngine/GameEngine/Assets/Models/Mixamo/Monster - Falling/Falling.dae");
		ModelManager::Instance()->LoadAnimation("C:/Users/User/Desktop/GameEngine/GameEngine/Assets/Models/Mixamo/Character - Jumping/CharacterJumping.dae");
		ModelManager::Instance()->LoadAnimation("C:/Users/User/Desktop/GameEngine/GameEngine/Assets/Models/Mixamo/Character - Walking/CharacterWalking.dae");
		ModelManager::Instance()->LoadAnimation("C:/Users/User/Desktop/GameEngine/GameEngine/Assets/Models/Mixamo/Soldier - Run/Slow Run.dae");
	}

	/*
	auto previewManager = PreviewManager::Instance();
	previewManager->ResgisterMaterial("DirLight1");
	previewManager->ResgisterMaterial("DirLight2");
	previewManager->ResgisterMaterial("DirLight3");
	previewManager->ResgisterMaterial("DirLight4");
	*/
}

Scene::~Scene()
{
	if (gScene)
	{
		gScene->release();
		gScene = nullptr;
	}

	if (gDispatcher)
	{
		gDispatcher->release();
		gDispatcher = nullptr;
	}

	if (gFoundation)
	{
		gFoundation->release();
		gFoundation = nullptr;
	}

	if (gPhysics)
	{
		gPhysics->release();
		gPhysics = nullptr;
	}
}

void Scene::Update(float deltaTime)
{
	auto resourceManager = ResourceManager::Instance();

	UpdateCamera(deltaTime);

	{ //Script System
		auto start = std::chrono::high_resolution_clock::now();
		//ScriptSystem::OnUpdate(m_Registry, deltaTime);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<ScriptSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ //Shape System
		auto start = std::chrono::high_resolution_clock::now();
		ShapeSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<ShapeSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ //Model System
		auto start = std::chrono::high_resolution_clock::now();
		ModelSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<ModelSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ //Animation System
		auto start = std::chrono::high_resolution_clock::now();
		AnimationSystem::OnUpdate(m_Registry, deltaTime);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<AnimationSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Transform System
		auto start = std::chrono::high_resolution_clock::now();
		TransformSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<TransformSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ //Box Collider System
		auto start = std::chrono::high_resolution_clock::now();
		BoxColliderSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<BoxColliderSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ //Sphere Collider System
		auto start = std::chrono::high_resolution_clock::now();
		SphereColliderSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<SphereColliderSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Convex Collider System
		auto start = std::chrono::high_resolution_clock::now();
		ConvexColliderSystem::OnUpdate(m_Registry, gPhysics);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<ConvexColliderSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Mesh Collider System
		auto start = std::chrono::high_resolution_clock::now();
		MeshColliderSystem::OnUpdate(m_Registry, gPhysics);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<MeshColliderSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Rigid Body Dynamic System
		auto start = std::chrono::high_resolution_clock::now();
		RigidbodyDynamicSystem::OnUpdate(m_Registry, gPhysics, gScene);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<RigidbodyDynamicSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Rigid Body Static System
		auto start = std::chrono::high_resolution_clock::now();
		RigidbodyStaticSystem::OnUpdate(m_Registry, gPhysics, gScene);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<RigidbodyStaticSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Rigid Body Dynamic Transform Update System
		auto start = std::chrono::high_resolution_clock::now();
		RigidbodyDynamicSystem::UpdateRigidbodyGlobalPose(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<RigidbodyDynamicSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Rigid Body Static Transform Update System
		auto start = std::chrono::high_resolution_clock::now();
		RigidbodyStaticSystem::UpdateRigidbodyGlobalPose(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<RigidbodyStaticSystem >()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Physics simulation
		if (true || GlobalSettings::GameViewActive)
		{
			auto start = std::chrono::high_resolution_clock::now();

			if (deltaTime < 0.1)
			{
				gScene->simulate(deltaTime);
				gScene->fetchResults(true);
			}

			auto end = std::chrono::high_resolution_clock::now();
			m_SystemTimes[Unique::typeIndex<PhysicsSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		}
	}

	{ // Rigid Body Dynamic Transform Fetch System
		auto start = std::chrono::high_resolution_clock::now();
		RigidbodyDynamicSystem::FetchRigidbodyGlobalPose(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<RigidbodyDynamicSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Transform System
		auto start = std::chrono::high_resolution_clock::now();
		TransformSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<TransformSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ //Default Collider System
		auto start = std::chrono::high_resolution_clock::now();
		DefaultColliderSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<DefaultColliderSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Frustum Culling System
		auto start = std::chrono::high_resolution_clock::now();
		FrustumCullingSystem::OnUpdate(m_Registry, m_SceneCamera);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<FrustumCullingSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Instance System
		auto start = std::chrono::high_resolution_clock::now();
		InstanceSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<InstanceSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Material System
		auto start = std::chrono::high_resolution_clock::now();
		MaterialSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<MaterialSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Direction Light System
		auto start = std::chrono::high_resolution_clock::now();
		DirlightSystem::OnUpdate(m_Registry, m_SceneCamera);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<DirlightSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Point Light System
		auto start = std::chrono::high_resolution_clock::now();
		PointLightSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<PointLightSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Spot Light System
		auto start = std::chrono::high_resolution_clock::now();
		SpotLightSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<SpotLightSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Water System
		auto start = std::chrono::high_resolution_clock::now();
		WaterSystem::OnUpdate(m_Registry, deltaTime);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<WaterSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	SoundManager::Instance()->SetListener(m_SceneCamera->GetPosition(), m_SceneCamera->GetDirection());

	{ // Audio System
		auto start = std::chrono::high_resolution_clock::now();
		AudioSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<AudioSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Transform System End
		auto start = std::chrono::high_resolution_clock::now();
		TransformSystem::OnEnd(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<TransformSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Model System End
		auto start = std::chrono::high_resolution_clock::now();
		ModelSystem::OnEnd(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<ModelSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Shape System End
		auto start = std::chrono::high_resolution_clock::now();
		ShapeSystem::OnEnd(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<ShapeSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	UpdateSystemTime(deltaTime);
}

void Scene::UpdateCamera(float deltaTime)
{
	m_SceneCamera->Update(deltaTime);
	m_SceneCamera->UpdateGLSL();
}

void Scene::UpdateSystemTime(float deltaTime)
{
	static float time = 0;
	static int counter = 0;
	time += deltaTime;
	counter++;

	if (time > 1)
	{
		for (auto& timeData : m_SystemTimes)
		{
			m_AverageSystemTimes[timeData.first] = timeData.second / counter;
			timeData.second = 0; 
		}

		std::cout << "fps = " << counter << std::endl;
		time = 0;
		counter = 0;
	}
}

void Scene::Serialize(const std::string& path)
{
	nlohmann::json data;
	data["name"] = name;
	data["registry"] = m_Registry->Serialize();

	std::ofstream output(path);
	
	if (output.is_open())
		output << data.dump(4);

	output.close();
}

void Scene::DeSerialize(const std::string& path)
{
	std::ifstream input(path);
	nlohmann::json data = nlohmann::json::parse(input);

	if (gScene)
	{
		gScene->release();
		gScene = nullptr;

		PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0, -9.81f, 0);
		gDispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = gDispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		gScene = gPhysics->createScene(sceneDesc);
	}

	AudioSystem::OnEnd(m_Registry);
	ScriptSystem::FreeScripts(m_Registry);

	this->name = data["name"];
	this->m_Registry = std::make_shared<Registry>();
	this->m_Registry->DeSerialize(data["registry"]);

	ScriptSystem::LoadScripts(m_Registry);
}