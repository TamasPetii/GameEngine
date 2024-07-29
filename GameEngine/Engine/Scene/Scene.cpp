#include "Scene.h"

Scene::Scene()
{
	auto textureManager = TextureManager::Instance();
	auto resourceManager = ResourceManager::Instance();

	m_Registry = std::make_shared<Registry>();
	m_SceneCamera = std::make_shared<Camera>();

	std::random_device rnd;
	std::mt19937_64 gen(rnd());
	std::uniform_real_distribution<float> dist(-1, 1);

	/*
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<RigidbodyComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "Floor";
		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(100, 1, 100);
		m_Registry->GetComponent<RigidbodyComponent>(entity).isStatic = true;
		m_Registry->GetComponent<MaterialComponent>(entity).diffuse = TextureManager::Instance()->LoadImageTexture("../Assets/Prototype2.png");
		m_Registry->GetComponent<MaterialComponent>(entity).textureScale = glm::vec2(20);
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<RigidbodyComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "Left Wall";
		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(-101, 21, 0);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(1, 20, 100);
		m_Registry->GetComponent<RigidbodyComponent>(entity).isStatic = true;
		m_Registry->GetComponent<MaterialComponent>(entity).diffuse = TextureManager::Instance()->LoadImageTexture("../Assets/Prototype3.png");
		m_Registry->GetComponent<MaterialComponent>(entity).textureScale = glm::vec2(20, 4);
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<RigidbodyComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "Right Wall";
		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(101, 21, 0);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(1, 20, 100);
		m_Registry->GetComponent<RigidbodyComponent>(entity).isStatic = true;
		m_Registry->GetComponent<MaterialComponent>(entity).diffuse = TextureManager::Instance()->LoadImageTexture("../Assets/Prototype3.png");
		m_Registry->GetComponent<MaterialComponent>(entity).textureScale = glm::vec2(20, 4);
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<RigidbodyComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "Back Wall";
		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(0, 21, -101);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(100, 20, 1);
		m_Registry->GetComponent<RigidbodyComponent>(entity).isStatic = true;
		m_Registry->GetComponent<MaterialComponent>(entity).diffuse = TextureManager::Instance()->LoadImageTexture("../Assets/Prototype3.png");
		m_Registry->GetComponent<MaterialComponent>(entity).textureScale = glm::vec2(20, 4);
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<SphereCollider>(entity);
		m_Registry->AddComponent<MeshCollider>(entity);
		m_Registry->AddComponent<ScriptComponent>(entity);

		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(0, 5.2, 0);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(5, 5, 5);
		m_Registry->GetComponent<MaterialComponent>(entity).color = glm::vec4(1, 1, 0, 1);
		m_Registry->GetComponent<MaterialComponent>(entity).diffuse = textureManager->LoadImageTexture("../Assets/Diffuse.jpg");
		m_Registry->GetComponent<MaterialComponent>(entity).normal  = textureManager->LoadImageTexture("../Assets/Models/Mixamo/textures/Vampire_normal.png");
		m_Registry->GetComponent<MaterialComponent>(entity).specular = textureManager->LoadImageTexture("../Assets/Specular.jpg");
		m_Registry->GetComponent<ScriptComponent>(entity).scripts.insert(std::make_pair("MoveEntityScript", nullptr));
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<DirlightComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->GetComponent<DirlightComponent>(entity).strength = 1.5f;
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

	for (int i = 0; i < 5; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<MaterialComponent>(entity);
		m_Registry->AddComponent<ShapeComponent>(entity);
		m_Registry->AddComponent<RigidbodyComponent>(entity);
		m_Registry->AddComponent<ScriptComponent>(entity);

		m_Registry->GetComponent<ShapeComponent>(entity).isInstanced = true;
		m_Registry->GetComponent<ShapeComponent>(entity).shape = resourceManager->GetGeometry("Cube");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(dist(gen) * 100, (dist(gen) + 1) * 100, dist(gen) * 100);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(1);
		m_Registry->GetComponent<MaterialComponent>(entity).color = glm::vec4((dist(gen) + 1) * 0.5, (dist(gen) + 1) * 0.5, (dist(gen) + 1) * 0.5, 1);
		m_Registry->GetComponent<MaterialComponent>(entity).useBloom = true;
		m_Registry->GetComponent<RigidbodyComponent>(entity).mass = 3 + (dist(gen) + 1) * 0.5 * 7;
		m_Registry->GetComponent<ScriptComponent>(entity).scripts.insert(std::make_pair("MoveEntityScript", nullptr));
	}

	for (int i = 0; i < 5; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<ModelComponent>(entity);

		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(dist(gen) * 100, (dist(gen) + 1) * 100, dist(gen) * 100);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(1);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(0.1);
		m_Registry->GetComponent<ModelComponent>(entity).isInstanced = true;
		m_Registry->GetComponent<ModelComponent>(entity).model = ModelManager::Instance()->LoadModel("../Assets/Models/Mixamo/Capoeira.dae");
	}

	for (int i = 0; i < 25; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<ModelComponent>(entity);

		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(dist(gen) * 100, (dist(gen) + 1) * 100, dist(gen) * 100);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(1);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(0.1);
		m_Registry->GetComponent<ModelComponent>(entity).isInstanced = false;
		m_Registry->GetComponent<ModelComponent>(entity).model = ModelManager::Instance()->LoadModel("../Assets/Models/Mixamo/Monster - Falling/Falling.dae");
	}

	for (int i = 0; i < 1; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->AddComponent<ScriptComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "ImGuiScript";
		m_Registry->GetComponent<ScriptComponent>(entity).scripts.insert(std::make_pair("ImGuiScript", nullptr));
	}

	for (int i = 0; i < 5; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<ModelComponent>(entity);
		m_Registry->AddComponent<AnimationComponent>(entity);
		m_Registry->AddComponent<ScriptComponent>(entity);
		m_Registry->GetComponent<TagComponent>(entity).name = "ModelAnimation";
		m_Registry->GetComponent<ModelComponent>(entity).isInstanced = false;
		m_Registry->GetComponent<ModelComponent>(entity).model = ModelManager::Instance()->LoadModel("../Assets/Models/Mixamo/Capoeira.dae");
		m_Registry->GetComponent<AnimationComponent>(entity).animation = ModelManager::Instance()->LoadAnimation("../Assets/Models/Mixamo/Capoeira.dae");
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(dist(gen) * 150, 0, dist(gen) * 150);
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(0.1);
		m_Registry->GetComponent<ScriptComponent>(entity).scripts.insert(std::make_pair("ChangeAnimationScript", nullptr));
	}

	for (int i = 0; i < 1; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<ModelComponent>(entity);
		m_Registry->GetComponent<ModelComponent>(entity).isInstanced = false;
		m_Registry->GetComponent<ModelComponent>(entity).model = ModelManager::Instance()->LoadModel("C:/Users/User/Desktop/GameEngine/GameEngine/Assets/Models/Mixamo/Monster - Falling/Falling.dae");
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(0.02);
	}

	for (int i = 0; i < 64; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<PointLightComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3((i / 10) * 3, 1, (i % 10) * 3);
		m_Registry->GetComponent<PointLightComponent>(entity).color = glm::vec3((dist(gen) + 1) * 0.5, (dist(gen) + 1) * 0.5, (dist(gen) + 1) * 0.5);
		m_Registry->SetFlag<PointLightComponent>(entity, REGENERATE_FLAG);
	}

	for (int i = 0; i < 64; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<SpotLightComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(-(i / 8) * 5, 1, -(i % 8) * 5);
		m_Registry->GetComponent<SpotLightComponent>(entity).color = glm::vec3((dist(gen) + 1) * 0.5, (dist(gen) + 1) * 0.5, (dist(gen) + 1) * 0.5);
		m_Registry->SetFlag<SpotLightComponent>(entity, REGENERATE_FLAG);
	}

	for (int i = 0; i < 1; i++)
	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<AudioComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->GetComponent<TransformComponent>(entity).translate = glm::vec3(i, 1, i);
		m_Registry->GetComponent<AudioComponent>(entity).soundSource = SoundManager::Instance()->LoadSoundSource("../Assets/Wake.mp3");
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TagComponent>(entity);
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<WaterComponent>(entity);

		m_Registry->GetComponent<TagComponent>(entity).name = "Water";
		m_Registry->GetComponent<TransformComponent>(entity).scale = glm::vec3(50, 0.01, 50);	
		m_Registry->GetComponent< WaterComponent>(entity).dudv = TextureManager::Instance()->LoadImageTexture("../Assets/dudv.png");
		m_Registry->GetComponent< WaterComponent>(entity).dudvScale = glm::vec2(5);
		m_Registry->GetComponent< WaterComponent>(entity).dudvWaveStrength = glm::vec2(0.025);
		m_Registry->GetComponent< WaterComponent>(entity).dudvMoveSpeed = 0.03f;
	
		TextureManager::Instance()->LoadImageTexture("../Assets/dudv2.jpg");
	}

	{
		auto entity = m_Registry->CreateEntity();
		m_Registry->AddComponent<TransformComponent>(entity);
		m_Registry->AddComponent<ModelComponent>(entity);

		m_Registry->GetComponent<ModelComponent>(entity).isInstanced = false;
		m_Registry->GetComponent<ModelComponent>(entity).model = ModelManager::Instance()->LoadModel("../Assets/Models/Wolf.obj");
	}

	ScriptSystem::LoadScripts(m_Registry);
	*/
}

Scene::~Scene()
{

}

void Scene::Update(float deltaTime)
{
	auto resourceManager = ResourceManager::Instance();

	UpdateCamera(deltaTime);

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

	{ //Script System
		auto start = std::chrono::high_resolution_clock::now();
		ScriptSystem::OnUpdate(m_Registry, deltaTime);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<ScriptSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ //Animation System
		auto start = std::chrono::high_resolution_clock::now();
		AnimationSystem::OnUpdate(m_Registry, deltaTime);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<AnimationSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ //Physics System 
		auto start = std::chrono::high_resolution_clock::now();
		PhysicsSystem::OnUpdate(m_Registry, deltaTime);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<PhysicsSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
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

	{ //Sphere Collider System
		auto start = std::chrono::high_resolution_clock::now();
		SphereColliderSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<SphereColliderSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Mesh Collider System
		auto start = std::chrono::high_resolution_clock::now();
		MeshColliderSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<MeshColliderSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ //Bounding Volume Hierachy Builder System
		auto start = std::chrono::high_resolution_clock::now();
		BvhSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<BvhSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ //Collision System with GJK
		auto start = std::chrono::high_resolution_clock::now();
		CollisionSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<CollisionSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
	}

	{ // Collision Response System with EPA
		auto start = std::chrono::high_resolution_clock::now();
		CollisionResponseSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<CollisionResponseSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
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

	{ // Transform System
		auto start = std::chrono::high_resolution_clock::now();
		TransformSystem::OnUpdate(m_Registry);
		auto end = std::chrono::high_resolution_clock::now();
		m_SystemTimes[Unique::typeIndex<TransformSystem>()] += static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
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

	AudioSystem::OnEnd(m_Registry);
	ScriptSystem::FreeScripts(m_Registry);

	this->name = data["name"];
	this->m_Registry = std::make_shared<Registry>();
	this->m_Registry->DeSerialize(data["registry"]);

	ScriptSystem::LoadScripts(m_Registry);
}