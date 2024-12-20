#pragma once
#include "EngineApi.h"
#include <string>
#include <algorithm>
#include <execution>
#include <typeindex>
#include <unordered_map>
#include "Registry/Registry.h"

class Geometry;
class ProgramGL;
class FramebufferGL;
class UniformBufferGL;
class ShaderStorageBufferGL;

class ENGINE_API ResourceManager
{
public:
	virtual ~ResourceManager();
	static ResourceManager* Instance();
	static void Destroy();

	auto GetUbo(const std::string& name) { return m_UniformBuffers[name]; }
	auto GetSsbo(const std::string& name) { return m_ShaderStorageBuffers[name]; }
	auto GetProgram(const std::string& name) { return m_Programs[name]; }
	auto GetGeometry(const std::string& name) { return m_Geometries[name]; }
	auto GetFbo(const std::string& name) { return m_FrameBuffers[name]; }
	auto& GetGeometryList() { return m_Geometries; }
	void RecalculateComponentsShaderStorageBuffers(std::shared_ptr<Registry> registry);

	template<typename T>
	unsigned int GetComponentSsboSize();
private:
	static ResourceManager* m_Instance;
	ResourceManager();
	void Init();
	void InitPrograms();
	void InitGeometries();
	void InitFrameBuffers();
	void InitUniformBuffers();
	void InitShaderStorageBuffers();
	std::unordered_map<std::string, std::shared_ptr<ProgramGL>> m_Programs;
	std::unordered_map<std::string, std::shared_ptr<Geometry>> m_Geometries;
	std::unordered_map<std::string, std::shared_ptr<FramebufferGL>> m_FrameBuffers;
	std::unordered_map<std::string, std::shared_ptr<UniformBufferGL>> m_UniformBuffers;	

	void GenerateComponentShaderStorageBuffer(const std::string& ssboName, __int64 size);
	template<typename T>
	void RecalculateComponentBuffer(std::shared_ptr<Registry> registry, std::function<void()> generateSsboFunction);
	template<typename T>
	void ResetUpdateFlagsInComponentPools(std::shared_ptr<PoolBase> pool);
	std::unordered_map<std::string, std::shared_ptr<ShaderStorageBufferGL>> m_ShaderStorageBuffers;
	std::unordered_map<std::type_index, unsigned int> m_ComponentSsboSizes;
};

template<typename T>
inline unsigned int ResourceManager::GetComponentSsboSize()
{
	return m_ComponentSsboSizes[typeid(T)];
}

template<typename T>
inline void ResourceManager::ResetUpdateFlagsInComponentPools(std::shared_ptr<PoolBase> pool)
{
	auto componentPool = std::static_pointer_cast<Pool<T>>(pool);

	if (!componentPool)
		return;

	std::for_each(std::execution::seq, componentPool->GetDenseEntitiesArray().begin(), componentPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			componentPool->SetFlag(entity, UPDATE_FLAG);
			componentPool->SetFlag(entity, REGENERATE_FLAG);
		}
	);
}

template<typename T>
inline void ResourceManager::RecalculateComponentBuffer(std::shared_ptr<Registry> registry, std::function<void()> generateSsboFunction)
{
	bool resizeBuffer = false;
	auto& typeIndex = typeid(T);
	auto pool = registry->GetComponentPool<T>();

	if (pool)
	{
		auto size = pool->GetSize();
		if (size > m_ComponentSsboSizes[typeIndex] || size < m_ComponentSsboSizes[typeIndex] - 25)
		{
			resizeBuffer = true;
			m_ComponentSsboSizes[typeIndex] = static_cast<unsigned int>(std::ceil(size / 25.f) + 0.05) * 25;
		}
	}
	else
	{
		if (m_ComponentSsboSizes[typeIndex] != 1)
		{
			resizeBuffer = true;
			m_ComponentSsboSizes[typeIndex] = 1;
		}
	}

	if (resizeBuffer)
	{
		generateSsboFunction();
		ResetUpdateFlagsInComponentPools<T>(pool);
		//std::cout << " pool size = " << (pool ? pool->GetSize() : 0) << " | New buffer size = " << m_ComponentSsboSizes[typeIndex] << std::endl;
	}
}