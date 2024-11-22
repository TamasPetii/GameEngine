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
	std::string modelPath = path;

	//This is for loading pc path free assets
	if (std::filesystem::exists(GlobalSettings::ProjectPath + "/" + path))
		modelPath = GlobalSettings::ProjectPath + "/" + path;

	if (!IsModelLoaded(modelPath))
	{
		auto model = std::make_shared<Model>();

		if (model->Load(modelPath))
		{
			m_Models[modelPath] = model;
			PreviewManager::Instance()->RegisterModel(modelPath);
		}
		else
		{
			m_Models[modelPath] = nullptr;
		}
	}

	return m_Models[modelPath];
}

bool ModelManager::IsModelLoaded(const std::string& path)
{
	return m_Models.find(path) != m_Models.end() && m_Models[path] != nullptr;
}

std::shared_ptr<Model> ModelManager::GetModel(const std::string& path)
{
	std::string modelPath = path;

	//This is for loading pc path free assets
	if (std::filesystem::exists(GlobalSettings::ProjectPath + "/" + path))
		modelPath = GlobalSettings::ProjectPath + "/" + path;

	if (IsModelLoaded(modelPath))
		return m_Models[modelPath];

	return nullptr;
}

bool ModelManager::IsAnimationLoaded(const std::string& path)
{
	return m_Animations.find(path) != m_Animations.end() && m_Animations[path] != nullptr;
}

std::shared_ptr<Animation> ModelManager::LoadAnimation(const std::string& path)
{
	std::string animationPath = path;

	//This is for loading pc path free assets
	if (std::filesystem::exists(GlobalSettings::ProjectPath + "/" + path))
		animationPath = GlobalSettings::ProjectPath + "/" + path;

	if (!IsAnimationLoaded(animationPath))
	{
		LoadModel(animationPath);

		auto animation = std::make_shared<Animation>();

		if (animation->Load(animationPath))
		{
			m_Animations[animationPath] = animation;
			PreviewManager::Instance()->ResgisterAnimation(animationPath);
		}
		else
		{
			m_Animations[animationPath] = nullptr;
		}
	}

	return m_Animations[animationPath];
}

std::shared_ptr<Animation> ModelManager::GetAnimation(const std::string& path)
{
	std::string animationPath = path;

	//This is for loading pc path free assets
	if (std::filesystem::exists(GlobalSettings::ProjectPath + "/" + path))
		animationPath = GlobalSettings::ProjectPath + "/" + path;

	if (IsAnimationLoaded(animationPath))
		return m_Animations[animationPath];

	return nullptr;
}