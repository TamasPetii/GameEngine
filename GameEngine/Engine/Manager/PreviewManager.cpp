#include "PreviewManager.h"

PreviewManager* PreviewManager::m_Instance = nullptr;

PreviewManager* PreviewManager::Instance()
{
	if (m_Instance == nullptr)
		m_Instance = new PreviewManager();
	return m_Instance;
}

void PreviewManager::Destroy()
{
	if (m_Instance != nullptr)
		delete m_Instance;
	m_Instance = nullptr;
}

void PreviewManager::RegisterModel(const std::string& path)
{
	m_ModelPreviewTextures.insert(std::make_pair(path, nullptr));
}

void PreviewManager::RegisterShape(const std::string& name)
{
	m_ShapePreviewTextures.insert(std::make_pair(name, nullptr));
}

void PreviewManager::ResgisterMaterial(const std::string& path)
{
	m_MaterialPreviewTextures.insert(std::make_pair(path, nullptr));
}