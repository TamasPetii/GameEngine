#pragma once
#include "EngineApi.h"
#include <string>
#include <unordered_map>
#include <Render/OpenGL/TextureGL.h>

class ENGINE_API PreviewManager
{
public:
	static PreviewManager* Instance();
	static void Destroy();

	bool HasModelPreview(const std::string& path);
	bool HasShapePreview(const std::string& name);
	bool HasMaterialPreview(const std::string& path);

	bool IsModelRegistered(const std::string& path);
	bool IsShapeRegistered(const std::string& name);
	bool IsMaterialRegistered(const std::string& path);

	std::shared_ptr<TextureGL> GetModelPreview(const std::string& path);
	std::shared_ptr<TextureGL> GetShapePreview(const std::string& name);
	std::shared_ptr<TextureGL> GetMaterialPreview(const std::string& path);

	void RegisterModel(const std::string& path);
	void RegisterShape(const std::string& name);
	void ResgisterMaterial(const std::string& path);

	auto& RefShapePreviews() { return m_ShapePreviewTextures; }
	auto& RefModelPreviews() { return m_ModelPreviewTextures; }
	auto& RefMaterialPreviews() { return m_MaterialPreviewTextures; }
private:
	PreviewManager() = default;
	static PreviewManager* m_Instance;
	std::unordered_map<std::string, std::shared_ptr<TextureGL>> m_ShapePreviewTextures;
	std::unordered_map<std::string, std::shared_ptr<TextureGL>> m_ModelPreviewTextures;
	std::unordered_map<std::string, std::shared_ptr<TextureGL>> m_MaterialPreviewTextures;
};

