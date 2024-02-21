#include "ResourceManager.h"

<<<<<<< HEAD
ResourceManager::ResourceManager()
{
	Init();
}

ResourceManager* ResourceManager::Instance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new ResourceManager();
		m_Instance->Init();
	}

	return m_Instance;
}

void ResourceManager::Destroy()
{
	if (m_Instance != nullptr)
		delete m_Instance;
	m_Instance = nullptr;
}

=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
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
	m_Programs["DeferredPre"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/DeferredPre.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/DeferredPre.frag")
		}
	));

	m_Programs["DeferredPreInstance"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/DeferredPreInstance.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/DeferredPreInstance.frag")
		}
	));

<<<<<<< HEAD
	m_Programs["DeferredPreModel"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/DeferredPreModel.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/DeferredPreModel.frag")
		}
	));

	m_Programs["DeferredPreModelInstance"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/DeferredPreModelInstance.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/DeferredPreModelInstance.frag")
		}
	));

=======
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8
	m_Programs["DeferredDir"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/DeferredDir.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/DeferredDir.frag")
		}
	));

	m_Programs["DeferredPoint"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/DeferredPoint.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/DeferredPoint.frag")
		}
	));

	m_Programs["DeferredStencil"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/DeferredStencil.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/DeferredStencil.frag")
		}
	));

	m_Programs["DeferredSpot"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/DeferredSpot.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/DeferredSpot.frag")
		}
	));

	m_Programs["ShadowDir"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/ShadowDir.vert"),
			ShaderGL(GL_GEOMETRY_SHADER, "Render/Shader/ShadowDir.geom"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/ShadowDir.frag")
		}
	));

	m_Programs["ShadowPoint"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/ShadowPoint.vert"),
			ShaderGL(GL_GEOMETRY_SHADER, "Render/Shader/ShadowPoint.geom"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/ShadowPoint.frag")
		}
	));

	m_Programs["Skybox"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/Skybox.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/Skybox.frag")
		}
	));

	m_Programs["Billboard"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/Billboard.vert"),
			ShaderGL(GL_GEOMETRY_SHADER, "Render/Shader/Billboard.geom"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/Billboard.frag")
		}
	));

	m_Programs["Wireframe"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/Wireframe.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/Wireframe.frag")
		}
	));

	m_Programs["BloomDown"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/BloomDown.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/BloomDown.frag")
		}
	));

	m_Programs["BloomUp"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/BloomUp.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/BloomUp.frag")
		}
	));

	m_Programs["Bloom"] = std::shared_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/Bloom.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/Bloom.frag")
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
}

void ResourceManager::InitUniformBuffers()
{
	m_UniformBuffers["CameraData"] = std::make_shared<UniformBufferGL>();
	m_UniformBuffers["CameraData"]->BufferStorage(6 * sizeof(glm::mat4) + sizeof(glm::vec4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
}

void ResourceManager::InitShaderStorageBuffers()
{
	m_ShaderStorageBuffers["TransformData"] = std::make_shared<ShaderStorageBufferGL>();
<<<<<<< HEAD
	m_ShaderStorageBuffers["TransformData"]->BufferStorage(25000 * sizeof(TransformGLSL), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["MaterialData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["MaterialData"]->BufferStorage(25000 * sizeof(MaterialGLSL), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["DirLightData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DirLightData"]->BufferStorage(25000 * sizeof(DirlightGLSL), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["DirLightBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DirLightBillboard"]->BufferStorage(25000 * sizeof(glm::vec4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["PointLightData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["PointLightData"]->BufferStorage(25000 * sizeof(PointLightGLSL), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["PointLightTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["PointLightTransform"]->BufferStorage(25000 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["PointLightBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["PointLightBillboard"]->BufferStorage(25000 * sizeof(glm::vec4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["SpotLightData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SpotLightData"]->BufferStorage(25000 * sizeof(SpotLightGLSL), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["SpotLightTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SpotLightTransform"]->BufferStorage(25000 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["SpotLightBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SpotLightBillboard"]->BufferStorage(25000 * sizeof(glm::vec4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["DefaultColliderTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DefaultColliderTransform"]->BufferStorage(25000 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["SphereColliderTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SphereColliderTransform"]->BufferStorage(25000 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
=======
	m_ShaderStorageBuffers["TransformData"]->BufferStorage(5000 * sizeof(TransformGLSL), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["MaterialData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["MaterialData"]->BufferStorage(5000 * sizeof(MaterialGLSL), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["DirLightData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DirLightData"]->BufferStorage(5000 * sizeof(DirlightGLSL), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["DirLightBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DirLightBillboard"]->BufferStorage(5000 * sizeof(glm::vec4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["PointLightData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["PointLightData"]->BufferStorage(5000 * sizeof(PointLightGLSL), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["PointLightTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["PointLightTransform"]->BufferStorage(5000 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["PointLightBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["PointLightBillboard"]->BufferStorage(5000 * sizeof(glm::vec4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["SpotLightData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SpotLightData"]->BufferStorage(5000 * sizeof(SpotLightGLSL), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["SpotLightTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SpotLightTransform"]->BufferStorage(5000 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["SpotLightBillboard"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SpotLightBillboard"]->BufferStorage(5000 * sizeof(glm::vec4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["DefaultColliderTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["DefaultColliderTransform"]->BufferStorage(5000 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);

	m_ShaderStorageBuffers["SphereColliderTransform"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["SphereColliderTransform"]->BufferStorage(5000 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
>>>>>>> dbe3498e9abeb8bac9c1ae1897a84e9f682ab8a8

	m_ShaderStorageBuffers["BvhTransformData"] = std::make_shared<ShaderStorageBufferGL>();
	m_ShaderStorageBuffers["BvhTransformData"]->BufferStorage(50000 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
}

void ResourceManager::InitFrameBuffers()
{
	{ //Main Framebuffer
		std::function<void(GLuint, const TextureFboSpecGL&)> idTextureClearFunction = [](GLuint textureID, const TextureFboSpecGL& spec) -> void {
			constexpr glm::uvec4 clearValue = glm::uvec4(std::numeric_limits<unsigned int>::max());
			glClearTexImage(textureID, 0, spec.format, spec.type, &clearValue);
			};

		std::function<std::any(GLuint, const TextureFboSpecGL&, GLint, GLint)> idTextureReadFunction = [](GLuint textureID, const TextureFboSpecGL& spec, GLint x, GLint y) -> std::any {
			glm::uvec4 pixelData;
			glGetTextureSubImage(textureID, 0, x, y, 0, 1, 1, 1, spec.format, spec.type, sizeof(pixelData), &pixelData);
			return std::make_any<glm::uvec4>(pixelData);
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
		idTextureSpec.internalFormat = GL_RGBA32UI;
		idTextureSpec.format = GL_RGBA_INTEGER;
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
}