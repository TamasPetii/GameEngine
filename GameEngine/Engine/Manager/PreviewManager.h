#pragma once
#include "EngineApi.h"
#include <string>
#include <unordered_map>
#include <Render/OpenGL/TextureGL.h>
#include "Registry/Component/AnimationComponent.h"

class ENGINE_API PreviewManager
{
public:
	static PreviewManager* Instance();
	static void Destroy();

	bool HasModelPreview(const std::string& path);
	bool HasShapePreview(const std::string& name);
	bool HasMaterialPreview(const std::string& path);
	bool HasAnimationPreview(const std::string& path);

	bool IsModelRegistered(const std::string& path);
	bool IsShapeRegistered(const std::string& name);
	bool IsMaterialRegistered(const std::string& path);
	bool IsAnimationRegistered(const std::string& path);

	std::shared_ptr<TextureGL> GetModelPreview(const std::string& path);
	std::shared_ptr<TextureGL> GetShapePreview(const std::string& name);
	std::shared_ptr<TextureGL> GetMaterialPreview(const std::string& path);
	std::shared_ptr<TextureGL> GetAnimationPreview(const std::string& path);

	void RegisterModel(const std::string& path);
	void RegisterShape(const std::string& name);
	void ResgisterMaterial(const std::string& path);
	void ResgisterAnimation(const std::string& path);

	auto& RefShapePreviews() { return m_ShapePreviewTextures; }
	auto& RefModelPreviews() { return m_ModelPreviewTextures; }
	auto& RefMaterialPreviews() { return m_MaterialPreviewTextures; }
	auto& RefAnimationPreviews() { return m_AnimationPreviewTextures; }

	AnimationComponent& RefAnimationPreviewComponent(const std::string& path);
private:
	PreviewManager() = default;
	static PreviewManager* m_Instance;
	std::unordered_map<std::string, std::shared_ptr<TextureGL>> m_ShapePreviewTextures;
	std::unordered_map<std::string, std::shared_ptr<TextureGL>> m_ModelPreviewTextures;
	std::unordered_map<std::string, std::shared_ptr<TextureGL>> m_MaterialPreviewTextures;
	std::unordered_map<std::string, std::shared_ptr<TextureGL>> m_AnimationPreviewTextures;
	std::unordered_map<std::string, AnimationComponent> m_AnimationPreviewComponents;
};

