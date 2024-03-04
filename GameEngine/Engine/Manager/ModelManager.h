#pragma once
#include "EngineApi.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <Model/Model.h>

class ENGINE_API ModelManager
{
public:
	static ModelManager* Instance();
	static void Destroy();
	std::shared_ptr<Model> LoadModel(const std::string& path);
	std::shared_ptr<Model> GetModel(const std::string& path);
	bool IsModelLoaded(const std::string& path);
	auto& GetModelsList() { return m_Models; }
private:
	ModelManager() = default;
	static ModelManager* m_Instance;
	std::unordered_map<std::string, std::shared_ptr<Model>> m_Models;
};
