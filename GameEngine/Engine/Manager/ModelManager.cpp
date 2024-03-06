#include "ModelManager.h"

ModelManager* ModelManager::m_Instance = nullptr;

ModelManager* ModelManager::Instance()
{
	if (m_Instance == nullptr)
		m_Instance = new ModelManager();
	return m_Instance;
}

void ModelManager::Destroy()
{
	if (m_Instance != nullptr)
		delete m_Instance;
	m_Instance = nullptr;
}

std::shared_ptr<Model> ModelManager::LoadModel(const std::string& path)
{
	if (!IsModelLoaded(path))
	{
		auto model = std::make_shared<Model>();
		model->Load(path);
		m_Models[path] = model;
	}

	return m_Models[path];
}

bool ModelManager::IsModelLoaded(const std::string& path)
{
	return m_Models.find(path) != m_Models.end();
}

std::shared_ptr<Model> ModelManager::GetModel(const std::string& path)
{
	if (IsModelLoaded(path))
		return m_Models[path];

	return nullptr;
}