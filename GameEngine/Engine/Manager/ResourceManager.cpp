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

	m_Programs["ModelAnimation"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "../Engine/Render/Shader/ModelAnimation.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "../Engine/Render/Shader/ModelAnimation.frag")
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

void ResourceManager::InitUniformBuffers()
{
	m_UniformBuffers["CameraData"] = std::make_shared<UniformBufferGL>();
	m_UniformBuffers["CameraData"]->BufferStorage(6 * sizeof(glm::mat4) + sizeof(glm::vec4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
}

void ResourceManager::InitShaderStorageBuffers()
{
	int count = 5000;
	GLenum flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	m_ShaderStorageBuffers["CameraData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["CameraData"]->BufferStorage(100 * sizeof(CameraGLSL), nullptr, flags);

	m_ShaderStorageBuffers["TransformData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["TransformData"]->BufferStorage(count * sizeof(TransformGLSL), nullptr, flags);

	m_ShaderStorageBuffers["MaterialData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["MaterialData"]->BufferStorage(count * sizeof(MaterialGLSL), nullptr, flags);

	m_ShaderStorageBuffers["DirLightData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DirLightData"]->BufferStorage(count * sizeof(DirlightGLSL), nullptr, flags);

	m_ShaderStorageBuffers["DirLightBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DirLightBillboard"]->BufferStorage(count * sizeof(glm::vec4), nullptr, flags);

	m_ShaderStorageBuffers["PointLightData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["PointLightData"]->BufferStorage(count * sizeof(PointLightGLSL), nullptr, flags);

	m_ShaderStorageBuffers["PointLightTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["PointLightTransform"]->BufferStorage(count * sizeof(glm::mat4), nullptr, flags);

	m_ShaderStorageBuffers["PointLightBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["PointLightBillboard"]->BufferStorage(count * sizeof(glm::vec4), nullptr, flags);

	m_ShaderStorageBuffers["SpotLightData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SpotLightData"]->BufferStorage(count * sizeof(SpotLightGLSL), nullptr, flags);

	m_ShaderStorageBuffers["SpotLightTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SpotLightTransform"]->BufferStorage(count * sizeof(glm::mat4), nullptr, flags);

	m_ShaderStorageBuffers["SpotLightBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SpotLightBillboard"]->BufferStorage(count * sizeof(glm::vec4), nullptr, flags);

	m_ShaderStorageBuffers["DefaultColliderTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DefaultColliderTransform"]->BufferStorage(count * sizeof(glm::mat4), nullptr, flags);

	m_ShaderStorageBuffers["BoxColliderTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["BoxColliderTransform"]->BufferStorage(count * sizeof(glm::mat4), nullptr, flags);

	m_ShaderStorageBuffers["SphereColliderTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SphereColliderTransform"]->BufferStorage(count * sizeof(glm::mat4), nullptr, flags);

	m_ShaderStorageBuffers["BvhTransformData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["BvhTransformData"]->BufferStorage(count * sizeof(glm::mat4), nullptr, flags);

	m_ShaderStorageBuffers["AudioBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["AudioBillboard"]->BufferStorage(count * sizeof(glm::vec4), nullptr, flags);

	m_ShaderStorageBuffers["WaterData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["WaterData"]->BufferStorage(count * sizeof(WaterGLSL), nullptr, flags);

	m_ShaderStorageBuffers["ShapeData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["ShapeData"]->BufferStorage(count * sizeof(glm::vec4), nullptr, flags);

	m_ShaderStorageBuffers["ModelData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["ModelData"]->BufferStorage(count * sizeof(glm::vec4), nullptr, flags);

	m_ShaderStorageBuffers["DirLightLines"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DirLightLines"]->BufferStorage(count * sizeof(DirlightLineGLSL), nullptr, flags);
}

void ResourceManager::InitFrameBuffers()
{
	{ //Main Framebuffer
		std::function<void(GLuint, const TextureFboSpecGL&)> idTextureClearFunction = [](GLuint textureID, const TextureFboSpecGL& spec) -> void {
			constexpr unsigned int clearValue = std::numeric_limits<unsigned int>::max();
			glClearTexImage(textureID, 0, spec.format, spec.type, &clearValue);
			};

		std::function<std::any(GLuint, const TextureFboSpecGL&, GLint, GLint)> idTextureReadFunction = [](GLuint textureID, const TextureFboSpecGL& spec, GLint x, GLint y) -> std::any {
			unsigned int pixelData;
			glGetTextureSubImage(textureID, 0, x, y, 0, 1, 1, 1, spec.format, spec.type, sizeof(pixelData), &pixelData);
			return std::make_any<unsigned int>(pixelData);
			};

		TextureFboSpecGL colorTextureSpec;
		colorTextureSpec.attachment = GL_COLOR_ATTACHMENT0;
		colorTextureSpec.textureType = GL_TEXTURE_2D;
		colorTextureSpec.internalFormat = GL_RGBA16F;
		colorTextureSpec.format = GL_RGBA;
		colorTextureSpec.type = GL_FLOAT;

		//(specular texture intensity, specular power, cast shadow)
		TextureFboSpecGL additionalTextureSpec;
		additionalTextureSpec.attachment = GL_COLOR_ATTACHMENT1;
		additionalTextureSpec.textureType = GL_TEXTURE_2D;
		additionalTextureSpec.internalFormat = GL_RGB8;
		additionalTextureSpec.format = GL_RGB;
		additionalTextureSpec.type = GL_FLOAT;

		TextureFboSpecGL normalTextureSpec;
		normalTextureSpec.attachment = GL_COLOR_ATTACHMENT2;
		normalTextureSpec.textureType = GL_TEXTURE_2D;
		normalTextureSpec.internalFormat = GL_RGB16F;
		normalTextureSpec.format = GL_RGB;
		normalTextureSpec.type = GL_FLOAT;

		TextureFboSpecGL idTextureSpec;
		idTextureSpec.attachment = GL_COLOR_ATTACHMENT3;
		idTextureSpec.textureType = GL_TEXTURE_2D;
		idTextureSpec.internalFormat = GL_R32UI;
		idTextureSpec.format = GL_RED_INTEGER;
		idTextureSpec.type = GL_UNSIGNED_INT;
		idTextureSpec.clearTextureFunction = idTextureClearFunction;
		idTextureSpec.readTextureFunction = idTextureReadFunction;

		TextureFboSpecGL mainTextureSpec;
		mainTextureSpec.attachment = GL_COLOR_ATTACHMENT4;
		mainTextureSpec.textureType = GL_TEXTURE_2D;
		mainTextureSpec.internalFormat = GL_RGBA16F;
		mainTextureSpec.format = GL_RGBA;
		mainTextureSpec.type = GL_FLOAT;

		TextureFboSpecGL bloomTextureSpec;
		bloomTextureSpec.attachment = GL_COLOR_ATTACHMENT5;
		bloomTextureSpec.textureType = GL_TEXTURE_2D;
		bloomTextureSpec.internalFormat = GL_RGBA16F;
		bloomTextureSpec.format = GL_RGBA;
		bloomTextureSpec.type = GL_FLOAT;

		TextureFboSpecGL depthTextureSpec;
		depthTextureSpec.attachment = GL_DEPTH_STENCIL_ATTACHMENT;
		depthTextureSpec.textureType = GL_TEXTURE_2D;
		depthTextureSpec.internalFormat = GL_DEPTH24_STENCIL8;
		depthTextureSpec.format = GL_DEPTH_STENCIL;
		depthTextureSpec.type = GL_UNSIGNED_INT_24_8;

		m_FrameBuffers["Main"] = std::make_shared<FramebufferGL>();
		m_FrameBuffers["Main"]->AttachTexture("color", colorTextureSpec);
		m_FrameBuffers["Main"]->AttachTexture("additional", additionalTextureSpec);
		m_FrameBuffers["Main"]->AttachTexture("normal", normalTextureSpec);
		m_FrameBuffers["Main"]->AttachTexture("id", idTextureSpec);
		m_FrameBuffers["Main"]->AttachTexture("main", mainTextureSpec);
		m_FrameBuffers["Main"]->AttachTexture("bloom", bloomTextureSpec);
		m_FrameBuffers["Main"]->AttachTexture("depth", depthTextureSpec);
		m_FrameBuffers["Main"]->CheckCompleteness();
	}

	{
		m_FrameBuffers["Bloom"] = std::make_shared<FramebufferGL>();
		glm::ivec2 size = m_FrameBuffers["Main"]->GetSize();

		for (unsigned int i = 0; i < 6; i++)
		{
			glm::ivec2 textureSize = size / 2;
			TextureFboSpecGL textureSpec;
			textureSpec.attachment = GL_COLOR_ATTACHMENT0;
			textureSpec.textureType = GL_TEXTURE_2D;
			textureSpec.internalFormat = GL_RGBA16F;
			textureSpec.format = GL_RGBA;
			textureSpec.type = GL_FLOAT;
			textureSpec.width = textureSize.x;
			textureSpec.height = textureSize.y;

			size = textureSize;
			std::string name = "bloom" + std::to_string(i);
			m_FrameBuffers["Bloom"]->AttachTexture(name, textureSpec);
		}

		m_FrameBuffers["Bloom"]->CheckCompleteness();
	}

	{
		TextureFboSpecGL textureSpec;
		textureSpec.attachment = GL_COLOR_ATTACHMENT0;
		textureSpec.textureType = GL_TEXTURE_2D;
		textureSpec.internalFormat = GL_RGB8;
		textureSpec.format = GL_RGB;
		textureSpec.type = GL_UNSIGNED_BYTE;

		TextureFboSpecGL depthTextureSpec;
		depthTextureSpec.attachment = GL_DEPTH_STENCIL_ATTACHMENT;
		depthTextureSpec.textureType = GL_TEXTURE_2D;
		depthTextureSpec.internalFormat = GL_DEPTH24_STENCIL8;
		depthTextureSpec.format = GL_DEPTH_STENCIL;
		depthTextureSpec.type = GL_UNSIGNED_INT_24_8;

		m_FrameBuffers["Preview"] = std::make_shared<FramebufferGL>(256, 256);
		m_FrameBuffers["Preview"]->AttachTexture("preview", textureSpec);
		m_FrameBuffers["Preview"]->AttachTexture("depth", depthTextureSpec);
		m_FrameBuffers["Preview"]->CheckCompleteness();
	}
}