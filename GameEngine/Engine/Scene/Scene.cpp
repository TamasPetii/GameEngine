#include "Scene.h"

using namespace physx;

Scene::Scene()
{
	auto textureManager = TextureManager::Instance();
	auto resourceManager = ResourceManager::Instance();

	std::random_device rnd;
	std::mt19937_64 gen(rnd());
	std::uniform_real_distribution<float> dist(-1, 1);

	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());

	m_Registry = std::make_shared<Registry>();
	DeSerialize("../Assets/NewScene.json");
}

Scene::~Scene()
{
	//Todo: Physics out of scene

	m_Registry = std::shared_ptr<Registry>();

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

	if (gPhysics)
	{
		gPhysics->release();
		gPhysics = nullptr;
	}

	if (gFoundation)
	{
		gFoundation->release();
		gFoundation = nullptr;
	}
}

void Scene::Update(float deltaTime)
{
	GlobalSettings::SkyboxRotation += GlobalSettings::SkyboxRotationSpeed * glm::vec3(GlobalSettings::SkyboxRotationDirection) * deltaTime;

	{ //Script System
		if (GlobalSettings::GameViewActive)
		{
			auto start = std::chrono::high_resolution_clock::now();
			ScriptSystem::OnUpdate(m_Registry, deltaTime);
			auto end = std::chrono::high_resolution_clock::now();
			m_SystemTimes[Unique::typeIndex<ScriptSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		}
	}

	auto resourceManager = ResourceManager::Instance();
	resourceManager->RecalculateComponentsShaderStorageBuffers(m_Registry);

	{ //Camera System
		auto start = std::chrono::high_resolution_clock::now();
		CameraSystem::OnUpdate(m_Registry, deltaTime);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<CameraSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	auto& cameraComponent = CameraSystem::GetMainCamera(m_Registry);
	SoundManager::Instance()->SetListener(cameraComponent.position, cameraComponent.direction);

	{ // Material System
		auto start = std::chrono::high_resolution_clock::now();
		MaterialSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<MaterialSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
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

	if (true || GlobalSettings::GameViewActive || GlobalSettings::EnableAnimationInEditor)
	{
		{ //Animation System
			auto start = std::chrono::high_resolution_clock::now();
			AnimationSystem::OnUpdate(m_Registry, deltaTime);
			auto end = std::chrono::high_resolution_clock::now();
			m_SystemTimes[Unique::typeIndex<AnimationSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		}
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


	if (GlobalSettings::GameViewActive || GlobalSettings::EnablePhysicsInEditor)
	{
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

		{ // Rigid Body Dynamic Transform Fetch System
			auto start = std::chrono::high_resolution_clock::now();
			RigidbodyDynamicSystem::FetchRigidbodyGlobalPose(m_Registry);
			auto end = std::chrono::high_resolution_clock::now();
			m_SystemTimes[Unique::typeIndex<RigidbodyDynamicSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		}
	}

	{ // Transform System
		auto start = std::chrono::high_resolution_clock::now();
		TransformSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<TransformSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Direction Light System
		auto start = std::chrono::high_resolution_clock::now();
		DirlightSystem::OnUpdate(m_Registry);
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

	{ //Default Collider System
		auto start = std::chrono::high_resolution_clock::now();
		DefaultColliderSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<DefaultColliderSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Frustum Culling System
		auto start = std::chrono::high_resolution_clock::now();
		FrustumCullingSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<FrustumCullingSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Shadow Culling System
		auto start = std::chrono::high_resolution_clock::now();
		ShadowCullingSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<ShadowCullingSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Instance System
		auto start = std::chrono::high_resolution_clock::now();
		InstanceSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<InstanceSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Water System
		auto start = std::chrono::high_resolution_clock::now();
		WaterSystem::OnUpdate(m_Registry, deltaTime);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<WaterSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

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

	{ // Box Collider System End
		auto start = std::chrono::high_resolution_clock::now();
		BoxColliderSystem::OnEnd(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<BoxColliderSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Sphere Collider System End
		auto start = std::chrono::high_resolution_clock::now();
		SphereColliderSystem::OnEnd(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<SphereColliderSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	UpdateSystemTime(deltaTime);
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

		time = 0;
		counter = 0;
	}
}

void Scene::Serialize(const std::string& path)
{
	nlohmann::json data;
	data["name"] = name;
	data["registry"] = m_Registry->Serialize();

	//BloomRenderer's data
	data["useBloom"] = BloomRenderer::useBloom;
	data["gamma"] = BloomRenderer::gamma;
	data["exposure"] = BloomRenderer::exposure;

	std::ofstream output(path);
	
	if (output.is_open())
		output << data.dump(4);

	output.close();
}

void Scene::DeSerialize(const std::string& path)
{
	std::ifstream input(path);
	nlohmann::json data = nlohmann::json::parse(input);

	AudioSystem::OnEnd(m_Registry);
	ScriptSystem::FreeScripts(m_Registry);

	this->path = path;
	this->name = data["name"];
	BloomRenderer::useBloom = data["useBloom"];
	BloomRenderer::gamma = data["gamma"];
	BloomRenderer::exposure = data["exposure"];

	this->m_Registry = std::make_shared<Registry>();

	if (gScene)
	{
		gScene->release();
		gScene = nullptr;
	}

	if (collisionCallback)
		delete collisionCallback;

	collisionCallback = new CollisionCallback(this->m_Registry);
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -9.81f, 0);
	gDispatcher = PxDefaultCpuDispatcherCreate(4);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = CollisionCallback::FilterShader;
	sceneDesc.simulationEventCallback = collisionCallback;
	gScene = gPhysics->createScene(sceneDesc);

	this->m_Registry->DeSerialize(data["registry"]);
	ScriptSystem::LoadScripts(this->m_Registry);
}