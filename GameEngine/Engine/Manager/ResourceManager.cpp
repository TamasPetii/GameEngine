#include "ResourceManager.h"

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

	m_Programs["DeferredStencil"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/DeferredStencil.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/DeferredStencil.frag")
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

	m_Programs["Test"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/Test.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/Test.frag")
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
	for (auto& [typeIndex, pool] : registry->GetPools())
	{
		if (pool != nullptr)
		{
			bool resizeBuffer = false;
			auto size = pool->GetPoolSize();

			if (size > m_ComponentSsboSizes[typeIndex] || size < m_ComponentSsboSizes[typeIndex] - 25)
			{
				resizeBuffer = true;
				m_ComponentSsboSizes[typeIndex] = static_cast<unsigned int>(std::ceil(size / 25.f) + 0.05) * 25;
			}

			if (resizeBuffer)
			{
				if (typeIndex == typeid(TransformComponent))
				{
					GenerateComponentShaderStorageBuffer("TransformData", m_ComponentSsboSizes[typeIndex] * sizeof(TransformGLSL));
					ResetUpdateFlagsInComponentPools<TransformComponent>(pool);
					std::cout << "TransformComponent buffer resized: pool size = " << size << " | New buffer size = " << m_ComponentSsboSizes[typeIndex] << std::endl;
				}
				else if (typeIndex == typeid(MaterialComponent))
				{
					GenerateComponentShaderStorageBuffer("MaterialData", m_ComponentSsboSizes[typeIndex] * sizeof(MaterialGLSL));
					ResetUpdateFlagsInComponentPools<MaterialComponent>(pool);
					std::cout << "MaterialComponent buffer resized: pool size = " << size << " | New buffer size = " << m_ComponentSsboSizes[typeIndex] << std::endl;
				}
				else if (typeIndex == typeid(DirlightComponent))
				{
					GenerateComponentShaderStorageBuffer("DirLightData", m_ComponentSsboSizes[typeIndex] * sizeof(DirlightGLSL));
					GenerateComponentShaderStorageBuffer("DirLightLines", m_ComponentSsboSizes[typeIndex] * sizeof(DirlightLineGLSL));
					GenerateComponentShaderStorageBuffer("DirLightBillboard", m_ComponentSsboSizes[typeIndex] * sizeof(glm::vec4));
					ResetUpdateFlagsInComponentPools<DirlightComponent>(pool);
					std::cout << "DirlightComponent buffer resized: pool size = " << size << " | New buffer size = " << m_ComponentSsboSizes[typeIndex] << std::endl;
				}
				else if (typeIndex == typeid(PointLightComponent))
				{
					GenerateComponentShaderStorageBuffer("PointLightData", m_ComponentSsboSizes[typeIndex] * sizeof(PointLightGLSL));
					GenerateComponentShaderStorageBuffer("PointLightTransform", m_ComponentSsboSizes[typeIndex] * sizeof(glm::mat4));
					GenerateComponentShaderStorageBuffer("PointLightBillboard", m_ComponentSsboSizes[typeIndex] * sizeof(glm::vec4));
					ResetUpdateFlagsInComponentPools<PointLightComponent>(pool);
					std::cout << "PointLightComponent buffer resized: pool size = " << size << " | New buffer size = " << m_ComponentSsboSizes[typeIndex] << std::endl;
				}
				else if (typeIndex == typeid(SpotLightComponent))
				{
					GenerateComponentShaderStorageBuffer("SpotLightData", m_ComponentSsboSizes[typeIndex] * sizeof(SpotLightGLSL));
					GenerateComponentShaderStorageBuffer("SpotLightTransform", m_ComponentSsboSizes[typeIndex] * sizeof(glm::mat4));
					GenerateComponentShaderStorageBuffer("SpotLightBillboard", m_ComponentSsboSizes[typeIndex] * sizeof(glm::vec4));
					ResetUpdateFlagsInComponentPools<SpotLightComponent>(pool);
					std::cout << "SpotLightComponent buffer resized: pool size = " << size << " | New buffer size = " << m_ComponentSsboSizes[typeIndex] << std::endl;
				}
				else if (typeIndex == typeid(DefaultCollider))
				{
					GenerateComponentShaderStorageBuffer("DefaultColliderTransform", m_ComponentSsboSizes[typeIndex] * sizeof(glm::mat4));
					ResetUpdateFlagsInComponentPools<DefaultCollider>(pool);
					std::cout << "DefaultCollider buffer resized: pool size = " << size << " | New buffer size = " << m_ComponentSsboSizes[typeIndex] << std::endl;
				}
				else if (typeIndex == typeid(BoxColliderComponent))
				{
					GenerateComponentShaderStorageBuffer("BoxColliderTransform", m_ComponentSsboSizes[typeIndex] * sizeof(glm::mat4));
					ResetUpdateFlagsInComponentPools<BoxColliderComponent>(pool);
					std::cout << "BoxColliderComponent buffer resized: pool size = " << size << " | New buffer size = " << m_ComponentSsboSizes[typeIndex] << std::endl;
				}
				else if (typeIndex == typeid(SphereColliderComponent))
				{
					GenerateComponentShaderStorageBuffer("SphereColliderTransform", m_ComponentSsboSizes[typeIndex] * sizeof(glm::mat4));
					ResetUpdateFlagsInComponentPools<SphereColliderComponent>(pool);
					std::cout << "SphereColliderComponent buffer resized: pool size = " << size << " | New buffer size = " << m_ComponentSsboSizes[typeIndex] << std::endl;
				}
				else if (typeIndex == typeid(AudioComponent))
				{
					GenerateComponentShaderStorageBuffer("AudioBillboard", m_ComponentSsboSizes[typeIndex] * sizeof(glm::vec4));
					ResetUpdateFlagsInComponentPools<AudioComponent>(pool);
					std::cout << "AudioComponent buffer resized: pool size = " << size << " | New buffer size = " << m_ComponentSsboSizes[typeIndex] << std::endl;
				}
				else if (typeIndex == typeid(WaterComponent))
				{
					GenerateComponentShaderStorageBuffer("WaterData", m_ComponentSsboSizes[typeIndex] * sizeof(WaterGLSL));
					ResetUpdateFlagsInComponentPools<WaterComponent>(pool);
					std::cout << "WaterComponent buffer resized: pool size = " << size << " | New buffer size = " << m_ComponentSsboSizes[typeIndex] << std::endl;
				}
				else if (typeIndex == typeid(ShapeComponent))
				{
					GenerateComponentShaderStorageBuffer("ShapeData", m_ComponentSsboSizes[typeIndex] * sizeof(glm::vec4));
					ResetUpdateFlagsInComponentPools<ShapeComponent>(pool);
					std::cout << "ShapeComponent buffer resized: pool size = " << size << " | New buffer size = " << m_ComponentSsboSizes[typeIndex] << std::endl;
				}
				else if (typeIndex == typeid(ModelComponent))
				{
					GenerateComponentShaderStorageBuffer("ModelData", m_ComponentSsboSizes[typeIndex] * sizeof(glm::vec4));
					ResetUpdateFlagsInComponentPools<ModelComponent>(pool);
					std::cout << "ModelComponent buffer resized: pool size = " << size << " | New buffer size = " << m_ComponentSsboSizes[typeIndex] << std::endl;
				}
			}
		}
	}
}

void ResourceManager::GenerateComponentShaderStorageBuffer(const std::string& ssboName, GLsizeiptr componentSize)
{
	static GLenum flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	static GLenum mapFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

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

	m_ShaderStorageBuffers["TransformData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["MaterialData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DirLightData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DirLightBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["PointLightData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["PointLightTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["PointLightBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SpotLightData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SpotLightTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SpotLightBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DefaultColliderTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["BoxColliderTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SphereColliderTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["AudioBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["WaterData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["ShapeData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["ModelData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DirLightLines"] = std::make_shared<ShaderStorageBufferGL>();
}

void ResourceManager::InitFrameBuffers()
{
	{ //Main Framebuffer
		std::function<void(GLuint, const TextureSpecGL&)> idTextureClearFunction = [](GLuint textureID, const TextureSpecGL& spec) -> void {
			constexpr unsigned int clearValue = std::numeric_limits<unsigned int>::max();
			glClearTexImage(textureID, 0, spec.format, spec.type, &clearValue);
			};

		std::function<std::any(GLuint, const TextureSpecGL&, GLint, GLint)> idTextureReadFunction = [](GLuint textureID, const TextureSpecGL& spec, GLint x, GLint y) -> std::any {
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
		idTextureSpec.clearTextureFunction = idTextureClearFunction;
		idTextureSpec.readTextureFunction = idTextureReadFunction;
		idTextureSpec.generateHandler = false;
		idTextureSpec.generateMipMap = false;
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