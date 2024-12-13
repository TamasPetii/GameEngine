#include "PreviewManager.h"
#include "Render/OpenGL/TextureGL.h"

PreviewManager* PreviewManager::m_Instance = nullptr;

PreviewManager* PreviewManager::Instance()
{
	if (m_Instance == nullptr)
		m_Instance = new PreviewManager();
	return m_Instance;
}

PreviewManager::~PreviewManager()
{
}

void PreviewManager::Destroy()
{
	if (m_Instance != nullptr)
		delete m_Instance;
	m_Instance = nullptr;
}

bool PreviewManager::HasModelPreview(const std::string& path)
{
	return IsModelRegistered(path) && m_ModelPreviewTextures[path] != nullptr;
}

bool PreviewManager::HasShapePreview(const std::string& name)
{
	return IsShapeRegistered(name) && m_ShapePreviewTextures[name] != nullptr;
}

bool PreviewManager::HasMaterialPreview(const std::string& path)
{
	return IsMaterialRegistered(path) && m_MaterialPreviewTextures[path] != nullptr;
}

bool PreviewManager::HasAnimationPreview(const std::string& path)
{
	return IsAnimationRegistered(path) && m_AnimationPreviewTextures[path] != nullptr;
}

bool PreviewManager::IsModelRegistered(const std::string& path)
{
	return m_ModelPreviewTextures.find(path) != m_ModelPreviewTextures.end();
}

bool PreviewManager::IsShapeRegistered(const std::string& name)
{
	return m_ShapePreviewTextures.find(name) != m_ShapePreviewTextures.end();
}

bool PreviewManager::IsMaterialRegistered(const std::string& path)
{
	return  m_MaterialPreviewTextures.find(path) != m_MaterialPreviewTextures.end();
}

bool PreviewManager::IsAnimationRegistered(const std::string& path)
{
	return  m_AnimationPreviewTextures.find(path) != m_AnimationPreviewTextures.end();
}

std::shared_ptr<TextureGL> PreviewManager::GetModelPreview(const std::string& path)
{
	if (IsModelRegistered(path))
		return m_ModelPreviewTextures[path];
	return nullptr;
}

std::shared_ptr<TextureGL> PreviewManager::GetShapePreview(const std::string& name)
{
	if (IsShapeRegistered(name))
		return m_ShapePreviewTextures[name];
	return nullptr;
}

std::shared_ptr<TextureGL> PreviewManager::GetMaterialPreview(const std::string& path)
{
	if (IsMaterialRegistered(path))
		return m_MaterialPreviewTextures[path];
	return nullptr;
}

std::shared_ptr<TextureGL> PreviewManager::GetAnimationPreview(const std::string& path)
{
	if (IsAnimationRegistered(path))
		return m_AnimationPreviewTextures[path];
	return nullptr;
}

AnimationComponent& PreviewManager::RefAnimationPreviewComponent(const std::string& path)
{
	static AnimationComponent component;
	if (IsAnimationRegistered(path))
		return m_AnimationPreviewComponents[path];
	return component;
}

void PreviewManager::RegisterModel(const std::string& path)
{
	if (!IsModelRegistered(path))
		m_ModelPreviewTextures.insert(std::make_pair(path, nullptr));
}

void PreviewManager::RegisterShape(const std::string& name)
{
	if (!IsShapeRegistered(name))
		m_ShapePreviewTextures.insert(std::make_pair(name, nullptr));
}

void PreviewManager::ResgisterMaterial(const std::string& path)
{
	if (!IsMaterialRegistered(path))
		m_MaterialPreviewTextures.insert(std::make_pair(path, nullptr));
}

void PreviewManager::ResgisterAnimation(const std::string& path)
{
	if (!IsAnimationRegistered(path))
	{
		m_AnimationPreviewTextures.insert(std::make_pair(path, nullptr));
		m_AnimationPreviewComponents.insert(std::make_pair(path, AnimationComponent{}));
	}
}
