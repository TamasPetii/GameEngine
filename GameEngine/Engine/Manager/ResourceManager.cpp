#include "ResourceManager.h"
#include <GL/glew.h>

#include "Render/OpenGL/ShaderGL.h"
#include "Render/OpenGL/ProgramGL.h"
#include "Render/OpenGL/FramebufferGL.h"
#include "Render/OpenGL/UniformBufferGL.h"
#include "Render/OpenGL/ShaderStorageBufferGL.h"

#include "Render/Geometry/Geometry.h"
#include "Render/Geometry/Cube.h"
#include "Render/Geometry/Sphere.h"
#include "Render/Geometry/Pyramid.h"
#include "Render/Geometry/Cylinder.h"
#include "Render/Geometry/Torus.h"
#include "Render/Geometry/Cone.h"

#include "PreviewManager.h"
#include "Registry/Component/Components.h"


ResourceManager* ResourceManager::m_Instance = nullptr;

ResourceManager::ResourceManager()
{
	Init();
}

ResourceManager* ResourceManager::Instance()
{
	if (m_Instance == nullptr)
		m_Instance = new ResourceManager();

	return m_Instance;
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Destroy()
{
	if (m_Instance != nullptr)
		delete m_Instance;
	m_Instance = nullptr;
}

void ResourceManager::Init()
{
	InitPrograms();
	InitGeometries();
	InitFrameBuffers();
	InitUniformBuffers();
	InitShaderStorageBuffers();
}

void ResourceManager::InitPrograms()
{
	m_Programs["DeferredDir"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/DeferredDir.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/DeferredDir.frag")
		}
	));

	m_Programs["DeferredPoint"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/DeferredPoint.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/DeferredPoint.frag")
		}
	));

	m_Programs["DeferredSpot"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/DeferredSpot.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/DeferredSpot.frag")
		}
	));

	m_Programs["ShadowDir"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/ShadowDir.vert"),
			ShaderGL(GL_GEOMETRY_SHADER, "../Engine/Render/Shader/ShadowDir.geom"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/ShadowDir.frag")
		}
	));

	m_Programs["ShadowPoint"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/ShadowPoint.vert"),
			ShaderGL(GL_GEOMETRY_SHADER, "../Engine/Render/Shader/ShadowPoint.geom"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/ShadowPoint.frag")
		}
	));

	m_Programs["ShadowSpot"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/ShadowSpot.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/ShadowSpot.frag")
		}
	));

	m_Programs["Skybox"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/Skybox.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/Skybox.frag")
		}
	));

	m_Programs["Billboard"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/Billboard.vert"),
			ShaderGL(GL_GEOMETRY_SHADER, "../Engine/Render/Shader/Billboard.geom"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/Billboard.frag")
		}
	));

	m_Programs["Wireframe"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/Wireframe.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/Wireframe.frag")
		}
	));

	m_Programs["BloomDown"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/BloomDown.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/BloomDown.frag")
		}
	));

	m_Programs["BloomUp"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/BloomUp.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/BloomUp.frag")
		}
	));

	m_Programs["Bloom"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/Bloom.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/Bloom.frag")
		}
	));

	m_Programs["WaterPre"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/WaterPre.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/WaterPre.frag")
		}
	));

	m_Programs["Water"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/Water.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/Water.frag")
		}
	));

	m_Programs["DeferredPre"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/DeferredPre.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/DeferredPre.frag")
		}
	));

	m_Programs["Preview"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/Preview.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/Preview.frag")
		}
	));

	m_Programs["DirLightLine"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/DirLightLine.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/DirLightLine.frag")
		}
	));

	m_Programs["PreviewBg"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/PreviewBg.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/PreviewBg.frag")
		}
	));

	m_Programs["PreviewAnimation"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/PreviewAnimation.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/PreviewAnimation.frag")
		}
	));

	m_Programs["DeferredAmbient"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/DeferredAmbient.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/DeferredAmbient.frag")
		}
	));
}

void ResourceManager::InitGeometries()
{
	m_Geometries["Cube"] = std::make_shared<Cube>();
	m_Geometries["Pyramid"] = std::make_shared<Pyramid>();
	m_Geometries["Cylinder"] = std::make_shared<Cylinder>();
	m_Geometries["Torus"] = std::make_shared<Torus>();
	m_Geometries["Sphere"] = std::make_shared<Sphere>();
	m_Geometries["Cone"] = std::make_shared<Cone>();
	m_Geometries["ProxySphere"] = std::make_shared<Sphere>(12);

	PreviewManager::Instance()->RegisterShape("Cube");
	PreviewManager::Instance()->RegisterShape("Pyramid");
	PreviewManager::Instance()->RegisterShape("Cylinder");
	PreviewManager::Instance()->RegisterShape("Torus");
	PreviewManager::Instance()->RegisterShape("Sphere");
	PreviewManager::Instance()->RegisterShape("Cone");
}

void ResourceManager::RecalculateComponentsShaderStorageBuffers(std::shared_ptr<Registry> registry)
{
	RecalculateComponentBuffer<CameraComponent>(registry,
		[&]() -> void {
			GenerateComponentShaderStorageBuffer("CameraBillboardData", m_ComponentSsboSizes[typeid(CameraComponent)] * sizeof(glm::vec4));
			GenerateComponentShaderStorageBuffer("CameraWireframeData", m_ComponentSsboSizes[typeid(CameraComponent)] * sizeof(glm::mat4));
			//std::cout << "CameraComponent buffer resized : ";
		}
	);

	RecalculateComponentBuffer<TransformComponent>(registry,
		[&]() -> void {
			GenerateComponentShaderStorageBuffer("TransformData", m_ComponentSsboSizes[typeid(TransformComponent)] * sizeof(TransformGLSL));
			//std::cout << "TransformComponent buffer resized : ";
		}
	);

	RecalculateComponentBuffer<MaterialComponent>(registry,
		[&]() -> void {
			GenerateComponentShaderStorageBuffer("MaterialData", m_ComponentSsboSizes[typeid(MaterialComponent)] * sizeof(MaterialGLSL));
			//std::cout << "MaterialComponent buffer resized : ";
		}
	);

	RecalculateComponentBuffer<DirlightComponent>(registry,
		[&]() -> void {
			GenerateComponentShaderStorageBuffer("DirLightData", m_ComponentSsboSizes[typeid(DirlightComponent)] * sizeof(DirlightGLSL));
			GenerateComponentShaderStorageBuffer("DirLightLines", m_ComponentSsboSizes[typeid(DirlightComponent)] * sizeof(DirlightLineGLSL));
			GenerateComponentShaderStorageBuffer("DirLightBillboard", m_ComponentSsboSizes[typeid(DirlightComponent)] * sizeof(glm::vec4));
			//std::cout << "DirlightComponent buffer resized : ";
		}
	);

	RecalculateComponentBuffer<PointLightComponent>(registry,
		[&]() -> void {
			GenerateComponentShaderStorageBuffer("PointLightData", m_ComponentSsboSizes[typeid(PointLightComponent)] * sizeof(PointLightGLSL));
			GenerateComponentShaderStorageBuffer("PointLightTransform", m_ComponentSsboSizes[typeid(PointLightComponent)] * sizeof(glm::mat4));
			GenerateComponentShaderStorageBuffer("PointLightBillboard", m_ComponentSsboSizes[typeid(PointLightComponent)] * sizeof(glm::vec4));
			//std::cout << "PointLightComponent buffer resized : ";
		}
	);

	RecalculateComponentBuffer<SpotLightComponent>(registry,
		[&]() -> void {
			GenerateComponentShaderStorageBuffer("SpotLightData", m_ComponentSsboSizes[typeid(SpotLightComponent)] * sizeof(SpotLightGLSL));
			GenerateComponentShaderStorageBuffer("SpotLightTransform", m_ComponentSsboSizes[typeid(SpotLightComponent)] * sizeof(glm::mat4));
			GenerateComponentShaderStorageBuffer("SpotLightBillboard", m_ComponentSsboSizes[typeid(SpotLightComponent)] * sizeof(glm::vec4));
			//std::cout << "SpotLightComponent buffer resized : ";
		}
	);

	RecalculateComponentBuffer<DefaultCollider>(registry,
		[&]() -> void {
			GenerateComponentShaderStorageBuffer("DefaultColliderTransform", m_ComponentSsboSizes[typeid(DefaultCollider)] * sizeof(glm::mat4));
			//std::cout << "DefaultCollider buffer resized : ";
		}
	);

	RecalculateComponentBuffer<BoxColliderComponent>(registry,
		[&]() -> void {
			GenerateComponentShaderStorageBuffer("BoxColliderTransform", m_ComponentSsboSizes[typeid(BoxColliderComponent)] * sizeof(glm::mat4));
			//std::cout << "BoxColliderComponent buffer resized : ";
		}
	);

	RecalculateComponentBuffer<SphereColliderComponent>(registry,
		[&]() -> void {
			GenerateComponentShaderStorageBuffer("SphereColliderTransform", m_ComponentSsboSizes[typeid(SphereColliderComponent)] * sizeof(glm::mat4));
			//std::cout << "SphereColliderComponent buffer resized : ";
		}
	);

	RecalculateComponentBuffer<AudioComponent>(registry,
		[&]() -> void {
			GenerateComponentShaderStorageBuffer("AudioBillboard", m_ComponentSsboSizes[typeid(AudioComponent)] * sizeof(glm::vec4));
			//std::cout << "AudioComponent buffer resized : ";
		}
	);

	RecalculateComponentBuffer<WaterComponent>(registry,
		[&]() -> void {
			GenerateComponentShaderStorageBuffer("WaterData", m_ComponentSsboSizes[typeid(WaterComponent)] * sizeof(WaterGLSL));
			//std::cout << "WaterComponent buffer resized : ";
		}
	);

	RecalculateComponentBuffer<ShapeComponent>(registry,
		[&]() -> void {
			GenerateComponentShaderStorageBuffer("ShapeData", m_ComponentSsboSizes[typeid(ShapeComponent)] * sizeof(glm::vec4));
			//std::cout << "ShapeComponent buffer resized : ";
		}
	);

	RecalculateComponentBuffer<ModelComponent>(registry,
		[&]() -> void {
			GenerateComponentShaderStorageBuffer("ModelData", m_ComponentSsboSizes[typeid(ModelComponent)] * sizeof(glm::vec4));
			//std::cout << "ModelComponent buffer resized : ";
		}
	);
}

void ResourceManager::GenerateComponentShaderStorageBuffer(const std::string& ssboName, __int64 componentSize)
{
	GLenum flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	GLenum mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	auto& ssbo = m_ShaderStorageBuffers[ssboName];

	if (ssbo && ssbo->GetBufferHandler() != nullptr)
		ssbo->UnMapBuffer();

	ssbo = std::make_shared<ShaderStorageBufferGL>();
	ssbo->BufferStorage(componentSize, nullptr, flags, mapFlags);
}

void ResourceManager::InitUniformBuffers()
{
}

void ResourceManager::InitShaderStorageBuffers()
{
	GLenum flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	GLenum mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	m_ShaderStorageBuffers["CameraData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["CameraData"]->BufferStorage(sizeof(CameraGLSL), nullptr, flags, mapFlags);
}

void ResourceManager::InitFrameBuffers()
{
	{ //Main Framebuffer
		std::function<void(unsigned int, const TextureSpecGL&)> idTextureClearFunction = [](unsigned int textureID, const TextureSpecGL& spec) -> void {
			constexpr unsigned int clearValue = std::numeric_limits<unsigned int>::max();
			glClearTexImage(textureID, 0, spec.format, spec.type, &clearValue);
			};

		std::function<std::any(unsigned int, const TextureSpecGL&, GLint, GLint)> idTextureReadFunction = [](unsigned int textureID, const TextureSpecGL& spec, GLint x, GLint y) -> std::any {
			unsigned int pixelData;
			glGetTextureSubImage(textureID, 0, x, y, 0, 1, 1, 1, spec.format, spec.type, sizeof(pixelData), &pixelData);
			return std::make_any<unsigned int>(pixelData);
			};

		TextureSpecGL colorTextureSpec;
		colorTextureSpec.attachment = GL_COLOR_ATTACHMENT0;
		colorTextureSpec.textureType = GL_TEXTURE_2D;
		colorTextureSpec.internalFormat = GL_RGBA16F;
		colorTextureSpec.format = GL_RGBA;
		colorTextureSpec.type = GL_FLOAT;
		colorTextureSpec.generateHandler = false;
		colorTextureSpec.generateMipMap = false;
		colorTextureSpec.paramTextureFunction = defaultFboParamTextureFunction;

		//(specular texture intensity, specular power, cast shadow)
		TextureSpecGL additionalTextureSpec;
		additionalTextureSpec.attachment = GL_COLOR_ATTACHMENT1;
		additionalTextureSpec.textureType = GL_TEXTURE_2D;
		additionalTextureSpec.internalFormat = GL_RGB8;
		additionalTextureSpec.format = GL_RGB;
		additionalTextureSpec.type = GL_FLOAT;
		additionalTextureSpec.generateHandler = false;
		additionalTextureSpec.generateMipMap = false;
		additionalTextureSpec.paramTextureFunction = defaultFboParamTextureFunction;

		TextureSpecGL normalTextureSpec;
		normalTextureSpec.attachment = GL_COLOR_ATTACHMENT2;
		normalTextureSpec.textureType = GL_TEXTURE_2D;
		normalTextureSpec.internalFormat = GL_RGB16F;
		normalTextureSpec.format = GL_RGB;
		normalTextureSpec.type = GL_FLOAT;
		normalTextureSpec.generateHandler = false;
		normalTextureSpec.generateMipMap = false;
		normalTextureSpec.paramTextureFunction = defaultFboParamTextureFunction;

		TextureSpecGL idTextureSpec;
		idTextureSpec.attachment = GL_COLOR_ATTACHMENT3;
		idTextureSpec.textureType = GL_TEXTURE_2D;
		idTextureSpec.internalFormat = GL_R32UI;
		idTextureSpec.format = GL_RED_INTEGER;
		idTextureSpec.type = GL_UNSIGNED_INT;
		idTextureSpec.generateHandler = false;
		idTextureSpec.generateMipMap = false;
		idTextureSpec.clearTextureFunction = idTextureClearFunction;
		idTextureSpec.readTextureFunction = idTextureReadFunction;
		idTextureSpec.paramTextureFunction = defaultFboParamTextureFunction;

		TextureSpecGL mainTextureSpec;
		mainTextureSpec.attachment = GL_COLOR_ATTACHMENT4;
		mainTextureSpec.textureType = GL_TEXTURE_2D;
		mainTextureSpec.internalFormat = GL_RGBA16F;
		mainTextureSpec.format = GL_RGBA;
		mainTextureSpec.type = GL_FLOAT;
		mainTextureSpec.generateHandler = false;
		mainTextureSpec.generateMipMap = false;
		mainTextureSpec.paramTextureFunction = defaultFboParamTextureFunction;

		TextureSpecGL bloomTextureSpec;
		bloomTextureSpec.attachment = GL_COLOR_ATTACHMENT5;
		bloomTextureSpec.textureType = GL_TEXTURE_2D;
		bloomTextureSpec.internalFormat = GL_RGBA16F;
		bloomTextureSpec.format = GL_RGBA;
		bloomTextureSpec.type = GL_FLOAT;
		bloomTextureSpec.generateHandler = false;
		bloomTextureSpec.generateMipMap = false;
		bloomTextureSpec.paramTextureFunction = defaultFboParamTextureFunction;

		TextureSpecGL positionTextureSpec;
		positionTextureSpec.attachment = GL_COLOR_ATTACHMENT6;
		positionTextureSpec.textureType = GL_TEXTURE_2D;
		positionTextureSpec.internalFormat = GL_RGB32F;
		positionTextureSpec.format = GL_RGB;
		positionTextureSpec.type = GL_FLOAT;
		positionTextureSpec.generateHandler = false;
		positionTextureSpec.generateMipMap = false;
		positionTextureSpec.paramTextureFunction = defaultFboParamTextureFunction;

		TextureSpecGL depthTextureSpec;
		depthTextureSpec.attachment = GL_DEPTH_ATTACHMENT;
		depthTextureSpec.textureType = GL_TEXTURE_2D;
		depthTextureSpec.internalFormat = GL_DEPTH_COMPONENT24;
		depthTextureSpec.format = GL_DEPTH_COMPONENT;
		depthTextureSpec.type = GL_FLOAT;
		depthTextureSpec.generateHandler = false;
		depthTextureSpec.generateMipMap = false;
		depthTextureSpec.paramTextureFunction = defaultFboParamTextureFunction;

		m_FrameBuffers["Main"] = std::make_shared<FramebufferGL>();
		m_FrameBuffers["Main"]->AttachTexture("color", colorTextureSpec);
		m_FrameBuffers["Main"]->AttachTexture("additional", additionalTextureSpec);
		m_FrameBuffers["Main"]->AttachTexture("normal", normalTextureSpec);
		m_FrameBuffers["Main"]->AttachTexture("id", idTextureSpec);
		m_FrameBuffers["Main"]->AttachTexture("main", mainTextureSpec);
		m_FrameBuffers["Main"]->AttachTexture("bloom", bloomTextureSpec);
		m_FrameBuffers["Main"]->AttachTexture("position", positionTextureSpec);
		m_FrameBuffers["Main"]->AttachTexture("depth", depthTextureSpec);
		m_FrameBuffers["Main"]->CheckCompleteness();
	}

	{
		m_FrameBuffers["Bloom"] = std::make_shared<FramebufferGL>();
		glm::ivec2 size = m_FrameBuffers["Main"]->GetSize();
		m_FrameBuffers["Bloom"]->useFboSizeAsTexSize = false;

		for (unsigned int i = 0; i < 6; i++)
		{
			glm::ivec2 textureSize = size / 2;
			TextureSpecGL textureSpec;
			textureSpec.attachment = GL_COLOR_ATTACHMENT0;
			textureSpec.textureType = GL_TEXTURE_2D;
			textureSpec.internalFormat = GL_RGBA16F;
			textureSpec.format = GL_RGBA;
			textureSpec.type = GL_FLOAT;
			textureSpec.width = textureSize.x;
			textureSpec.height = textureSize.y;
			textureSpec.generateMipMap = false;
			textureSpec.generateHandler = false;
			textureSpec.paramTextureFunction = defaultFboParamTextureFunction;

			size = textureSize;
			std::string name = "bloom" + std::to_string(i);
			m_FrameBuffers["Bloom"]->AttachTexture(name, textureSpec);
		}

		m_FrameBuffers["Bloom"]->CheckCompleteness();
	}

	{
		TextureSpecGL textureSpec;
		textureSpec.attachment = GL_COLOR_ATTACHMENT0;
		textureSpec.textureType = GL_TEXTURE_2D;
		textureSpec.internalFormat = GL_RGB8;
		textureSpec.format = GL_RGB;
		textureSpec.type = GL_UNSIGNED_BYTE;
		textureSpec.generateMipMap = false;
		textureSpec.generateHandler = false;
		textureSpec.paramTextureFunction = defaultFboParamTextureFunction;

		TextureSpecGL depthTextureSpec;
		depthTextureSpec.attachment = GL_DEPTH_ATTACHMENT;
		depthTextureSpec.textureType = GL_TEXTURE_2D;
		depthTextureSpec.internalFormat = GL_DEPTH_COMPONENT24;
		depthTextureSpec.format = GL_DEPTH_COMPONENT;
		depthTextureSpec.type = GL_FLOAT;
		depthTextureSpec.generateMipMap = false;
		depthTextureSpec.generateHandler = false;
		depthTextureSpec.paramTextureFunction = defaultFboParamTextureFunction;

		m_FrameBuffers["Preview"] = std::make_shared<FramebufferGL>(256, 256);
		m_FrameBuffers["Preview"]->AttachTexture("preview", textureSpec);
		m_FrameBuffers["Preview"]->AttachTexture("depth", depthTextureSpec);
		m_FrameBuffers["Preview"]->CheckCompleteness();
	}
}