#include "Scene.h"

#include <chrono>
#include <fstream>

#include "Registry/Registry.h"
#include "Registry/System/Systems.h"
#include "Registry/Component/Components.h"
#include "Settings/GlobalSettings.h"
#include "Collision/CollisionCallback.h"
#include "Render/SkyboxRenderer.h"
#include "Render/BloomRenderer.h"
#include "Manager/SoundManager.h"
#include <random>

Scene::Scene()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale());

	m_Registry = std::make_shared<Registry>();
	DeSerialize("../Assets/Scene/NewScene.json");
}

Scene::~Scene()
{
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
	SkyboxRenderer::SkyboxRotation += SkyboxRenderer::SkyboxRotationSpeed * glm::vec3(SkyboxRenderer::SkyboxRotationDirection) * deltaTime;

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


	if (GlobalSettings::GameViewActive || GlobalSettings::EnablePhysicsInEditor)
	{
		{ // Rigid Body Dynamic Transform Update System
			auto start = std::chrono::high_resolution_clock::now();
			RigidbodyDynamicSystem::UpdateRigidbodyGlobalPose(m_Registry, physicsSimulationFetchInit);
			auto end = std::chrono::high_resolution_clock::now();
			m_SystemTimes[Unique::typeIndex<RigidbodyDynamicSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		}

		{ // Rigid Body Static Transform Update System
			auto start = std::chrono::high_resolution_clock::now();
			RigidbodyStaticSystem::UpdateRigidbodyGlobalPose(m_Registry, physicsSimulationFetchInit);
			auto end = std::chrono::high_resolution_clock::now();
			m_SystemTimes[Unique::typeIndex<RigidbodyStaticSystem >()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		}

		{
			auto start = std::chrono::high_resolution_clock::now();
			PhysicsSystem::OnUpdate(gScene, deltaTime);
			auto end = std::chrono::high_resolution_clock::now();
			m_SystemTimes[Unique::typeIndex<PhysicsSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		}

		{ // Rigid Body Dynamic Transform Fetch System
			auto start = std::chrono::high_resolution_clock::now();
			RigidbodyDynamicSystem::FetchRigidbodyGlobalPose(m_Registry);
			auto end = std::chrono::high_resolution_clock::now();
			m_SystemTimes[Unique::typeIndex<RigidbodyDynamicSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
		}

		physicsSimulationFetchInit = false;
	}
	else
	{
		physicsSimulationFetchInit = true;
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

	{ // Mesh Collider System End
		auto start = std::chrono::high_resolution_clock::now();
		MeshColliderSystem::OnEnd(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<MeshColliderSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Convex Collider System End
		auto start = std::chrono::high_resolution_clock::now();
		ConvexColliderSystem::OnEnd(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<ConvexColliderSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
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
			m_AverageSystemTimes[timeData.first] = (timeData.second / counter) / 1000.0;
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
	data["BloomRenderer"] = BloomRenderer::Serialize();
	data["SkyboxRenderer"] = SkyboxRenderer::Serialize();
	data["PhysicsSystem"] = PhysicsSystem::Serialize();

	std::ofstream output(path);

	if (output.is_open())
		output << data.dump(4);

	output.close();
}

void Scene::DeSerialize(const std::string& path)
{
	std::ifstream input(path);
	nlohmann::json data = nlohmann::json::parse(input);

	this->path = path;
	this->name = data["name"];
	this->m_Registry = std::make_shared<Registry>();
	this->m_Registry->DeSerialize(data["registry"]);

	if (data.find("BloomRenderer") != data.end())
		BloomRenderer::DeSerialize(data["BloomRenderer"]);

	if (data.find("SkyboxRenderer") != data.end())
		SkyboxRenderer::DeSerialize(data["SkyboxRenderer"]);

	if (data.find("PhysicsSystem") != data.end())
		PhysicsSystem::DeSerialize(data["PhysicsSystem"]);

	if (gScene)
	{
		gScene->release();
		gScene = nullptr;
	}

	int maxThreads = std::thread::hardware_concurrency();
	collisionCallback = new CollisionCallback(this->m_Registry);
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(PhysicsSystem::gravity.x, PhysicsSystem::gravity.y, PhysicsSystem::gravity.z);
	gDispatcher = PxDefaultCpuDispatcherCreate(maxThreads);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = CollisionCallback::FilterShader;
	sceneDesc.simulationEventCallback = collisionCallback;
	gScene = gPhysics->createScene(sceneDesc);
}

void Scene::StartGame()
{
	physicsSimulationFetchInit = true;
	ScriptSystem::LoadLib();
	ScriptSystem::LoadScripts(m_Registry);
	ScriptSystem::OnStart(m_Registry);
}

void Scene::EndGame()
{
	AudioSystem::OnEnd(m_Registry);
	ScriptSystem::FreeScripts(m_Registry);

	/*Important!!!
	Registry, and all its references have to be deleted before destroying the loaded script dll.
	If we dont do it this way the scripts might call registry->AddComponent, and it might creates a component pool.
	After destroying the script dll and registry still holds the component pool objects, which memory is unaccessable
	*/

	if (collisionCallback)
	{
		delete collisionCallback;
		collisionCallback = nullptr;
	}

	m_Registry = std::shared_ptr<Registry>();

	ScriptSystem::FreeLib();
}