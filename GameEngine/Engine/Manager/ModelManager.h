#pragma once
#include "EngineApi.h"
#include <memory>
#include <string>
#include <unordered_map>

class Model;
class Animation;

class ENGINE_API ModelManager
{
public:
	virtual ~ModelManager();
	static ModelManager* Instance();
	static void Destroy();
	std::shared_ptr<Model> LoadModel(const std::string& path);
	std::shared_ptr<Animation> LoadAnimation(const std::string& path);

	std::shared_ptr<Animation> GetAnimation(const std::string& path);
	std::shared_ptr<Model> GetModel(const std::string& path);

	auto& GetModelsList() { return m_Models; }
	auto& GetAnimationList() { return m_Animations; }
private:
	bool IsModelLoaded(const std::string& path);
	bool IsAnimationLoaded(const std::string& path);
	ModelManager() = default;
	static ModelManager* m_Instance;
	std::unordered_map<std::string, std::shared_ptr<Model>> m_Models;
	std::unordered_map<std::string, std::shared_ptr<Animation>> m_Animations;
};
