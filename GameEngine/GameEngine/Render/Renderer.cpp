#include "Renderer.h"

Renderer* Renderer::Instance()
{
	if (m_Instance == nullptr)
		m_Instance = new Renderer();

	return m_Instance;
}

void Renderer::Destroy()
{
	if (m_Instance != nullptr)
	{
		delete m_Instance;
		m_Instance = nullptr;
	}
}

Renderer::Renderer()
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	m_Camera = std::make_shared<Camera>();
	m_CameraUbo = std::make_shared<UniformBufferGL>();
	m_CameraUbo->BufferStorage(6 * sizeof(glm::mat4) + sizeof(glm::vec4), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
	
	InitPrograms();
	InitCube();
	InitFbo();
	InitTexture();
	InitTransforms();
}

Renderer::~Renderer()
{

}

void Renderer::Update()
{
	static int frames = 0;
	static float time = 0;
	static float lastTime = static_cast<float>(glfwGetTime());
	float currentTime = static_cast<float>(glfwGetTime());
	float deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	time += deltaTime;
	frames += 1;

	if (time > 1)
	{
		std::cout << frames << std::endl;
		time = 0;
		frames = 0;
	}

	m_Camera->Update(deltaTime);

	std::vector<glm::mat4> cameraMatrices{
		m_Camera->GetView(),
		m_Camera->GetViewInv(),
		m_Camera->GetProj(),
		m_Camera->GetProjInv(),
		m_Camera->GetViewProj(),
		m_Camera->GetViewProjInv()
	};

	m_CameraUbo->BufferSubStorage(0, 6 * sizeof(glm::mat4), cameraMatrices.data());
	m_CameraUbo->BufferSubStorage(6 * sizeof(glm::mat4), sizeof(glm::vec4), &m_Camera->GetPosition());

	auto handler = static_cast<TransformGLSL*>(m_TransformSsbo->MapBuffer(GL_WRITE_ONLY));
	auto transformPool = m_Registry.GetComponentPool<TransformComponent>();

	std::for_each(std::execution::par, transformPool->GetDenseEntitiesArray().begin(), transformPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {		
			if (transformPool->IsFlagSet(entity, UPDATE_FLAG))
			{
				int index = &entity - &transformPool->GetDenseEntitiesArray()[0];
				auto& transformComponent = transformPool->GetComponent(entity);

				transformComponent.modelTransform = glm::translate(transformComponent.translate)
					* glm::rotate(glm::radians(transformComponent.rotate.z), glm::vec3(0, 0, 1))
					* glm::rotate(glm::radians(transformComponent.rotate.y), glm::vec3(0, 1, 0))
					* glm::rotate(glm::radians(transformComponent.rotate.x), glm::vec3(1, 0, 0))
					* glm::scale(transformComponent.scale);

				transformComponent.modelTransformIT = glm::inverse(glm::transpose(transformComponent.modelTransform));
				transformPool->ResFlag(entity, UPDATE_FLAG);

				handler[index] = TransformGLSL(transformComponent);
			}
		}
	);

	m_TransformSsbo->UnMapBuffer();
}

void Renderer::Render()
{
	m_Framebuffer->Clear();
	m_Framebuffer->Bind();
	m_Framebuffer->ActivateTextures({ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 });

	//glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
	//glViewport(0, 0, m_Camera->GetWidth(), m_Camera->GetHeight());
	//glNamedFramebufferDrawBuffer(m_fboID, GL_COLOR_ATTACHMENT0);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_CameraUbo->BindBufferBase(0);
	m_TransformSsbo->BindBufferBase(1);
	auto& program = m_Programs["Test"];
	program->Bind();
	program->SetTexture("tex", 0, m_texture->GetTextureID());

	m_cubeVao->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr, count);
	m_cubeVao->UnBind();

	program->UnBind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::InitPrograms()
{
	//ShaderGL::PreLoadShader(std::filesystem::absolute("Render/Shader/Struct.glsl").string());

	m_Programs["Test"] = std::unique_ptr<ProgramGL>(new ProgramGL(
		{
			ShaderGL(GL_VERTEX_SHADER, "Render/Shader/Test.vert"),
			ShaderGL(GL_FRAGMENT_SHADER, "Render/Shader/Test.frag")
		}
	));
}

void Renderer::InitCube()
{
	std::vector<Vertex> vertices;

	//front									 
	vertices.push_back({ glm::vec3(-1.0, -1.0, +1.0), glm::vec3(0, 0, 1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+1.0, -1.0, +1.0), glm::vec3(0, 0, 1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-1.0, +1.0, +1.0), glm::vec3(0, 0, 1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+1.0, +1.0, +1.0), glm::vec3(0, 0, 1), glm::vec2(1, 1) });
	//back							1.0	  1.0	1.0
	vertices.push_back({ glm::vec3(+1.0, -1.0, -1.0), glm::vec3(0, 0, -1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-1.0, -1.0, -1.0), glm::vec3(0, 0, -1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+1.0, +1.0, -1.0), glm::vec3(0, 0, -1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-1.0, +1.0, -1.0), glm::vec3(0, 0, -1), glm::vec2(1, 1) });
	//right							1.0	  1.0	1.0
	vertices.push_back({ glm::vec3(+1.0, -1.0, +1.0), glm::vec3(1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+1.0, -1.0, -1.0), glm::vec3(1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+1.0, +1.0, +1.0), glm::vec3(1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+1.0, +1.0, -1.0), glm::vec3(1, 0, 0), glm::vec2(1, 1) });
	//left							1.0	  1.0	1.0
	vertices.push_back({ glm::vec3(-1.0, -1.0, -1.0), glm::vec3(-1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-1.0, -1.0, +1.0), glm::vec3(-1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-1.0, +1.0, -1.0), glm::vec3(-1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-1.0, +1.0, +1.0), glm::vec3(-1, 0, 0), glm::vec2(1, 1) });
	//top							1.0	  1.0	1.0
	vertices.push_back({ glm::vec3(-1.0, +1.0, +1.0), glm::vec3(0, 1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+1.0, +1.0, +1.0), glm::vec3(0, 1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-1.0, +1.0, -1.0), glm::vec3(0, 1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+1.0, +1.0, -1.0), glm::vec3(0, 1, 0), glm::vec2(1, 1) });
	//bottom						1.0	 
	vertices.push_back({ glm::vec3(-1.0, -1, -1), glm::vec3(0, -1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+1.0, -1, -1), glm::vec3(0, -1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-1.0, -1, +1), glm::vec3(0, -1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+1.0, -1, +1), glm::vec3(0, -1, 0), glm::vec2(1, 1) });

	std::vector<GLuint> indices(36);
	int index = 0;
	for (unsigned int i = 0; i < 6 * 4; i += 4)
	{
		indices[index + 0] = i + 0;
		indices[index + 1] = i + 1;
		indices[index + 2] = i + 2;
		indices[index + 3] = i + 1;
		indices[index + 4] = i + 3;
		indices[index + 5] = i + 2;
		index += 6;
	}

	m_cubeVao = std::make_shared<VertexArrayGL>();
	m_cubeIbo = std::make_shared<IndexBufferGL>();
	m_cubeVbo = std::make_shared<VertexBufferGL>();
	m_cubeVbo->BufferStorage(vertices.size() * sizeof(Vertex), vertices.data(), GL_NONE);
	m_cubeIbo->BufferStorage(indices.size() * sizeof(GLuint),  indices.data(),  GL_NONE);
	m_cubeVao->AttachIndexBuffer(m_cubeIbo);
	m_cubeVao->AttachVertexBuffer(m_cubeVbo, sizeof(Vertex), 0);
	m_cubeVao->LinkAttribute(0, 0, 3, GL_FLOAT, (GLuint)(0 * sizeof(glm::vec3)));
	m_cubeVao->LinkAttribute(0, 1, 3, GL_FLOAT, (GLuint)(1 * sizeof(glm::vec3)));
	m_cubeVao->LinkAttribute(0, 2, 2, GL_FLOAT, (GLuint)(2 * sizeof(glm::vec3)));
}

void Renderer::InitFbo()
{
	glCreateFramebuffers(1, &m_fboID);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_fboColorID);
	glTextureParameteri(m_fboColorID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_fboColorID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_fboColorID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(m_fboColorID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTextureStorage2D(m_fboColorID, 1, GL_RGBA16F, m_Camera->GetWidth(), m_Camera->GetHeight());
	glNamedFramebufferTexture(m_fboID, GL_COLOR_ATTACHMENT0, m_fboColorID, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_fboDepthID);
	glTextureParameteri(m_fboDepthID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(m_fboDepthID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(m_fboDepthID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(m_fboDepthID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTextureStorage2D(m_fboDepthID, 1, GL_DEPTH24_STENCIL8, m_Camera->GetWidth(), m_Camera->GetHeight());
	glNamedFramebufferTexture(m_fboID, GL_DEPTH_STENCIL_ATTACHMENT, m_fboDepthID, 0);

	if (glCheckNamedFramebufferStatus(m_fboID, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("Error occurred while creating frame buffer!");

	{
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

		TextureFboSpecGL depthTextureSpec;
		depthTextureSpec.attachment = GL_DEPTH_STENCIL_ATTACHMENT;
		depthTextureSpec.textureType = GL_TEXTURE_2D;
		depthTextureSpec.internalFormat = GL_DEPTH24_STENCIL8;
		depthTextureSpec.format = GL_DEPTH_STENCIL;
		depthTextureSpec.type = GL_UNSIGNED_INT_24_8;

		TextureFboSpecGL idTextureSpec;
		idTextureSpec.attachment = GL_COLOR_ATTACHMENT1;
		idTextureSpec.textureType = GL_TEXTURE_2D;
		idTextureSpec.internalFormat = GL_R32UI;
		idTextureSpec.format = GL_RED_INTEGER;
		idTextureSpec.type = GL_UNSIGNED_INT;
		idTextureSpec.clearTextureFunction = idTextureClearFunction;
		idTextureSpec.readTextureFunction = idTextureReadFunction;
			
		m_Framebuffer = std::make_shared<FramebufferGL>(m_Camera->GetWidth(), m_Camera->GetHeight());
		m_Framebuffer->AttachTexture("color", colorTextureSpec);
		m_Framebuffer->AttachTexture("id",    idTextureSpec);
		m_Framebuffer->AttachTexture("depth", depthTextureSpec);
		m_Framebuffer->CheckCompleteness();
	}

}

void Renderer::InitTexture()
{
	int width, height, bpp;
	stbi_set_flip_vertically_on_load(false);
	GLubyte* data = stbi_load("../Assets/wood.jpg", &width, &height, &bpp, 0);

	TextureSpecGL spec;
	spec.width = width;
	spec.height = height;
	spec.internalFormat = bpp == 3 ? GL_RGB8 : GL_RGBA8;
	spec.format = bpp == 3 ? GL_RGB : GL_RGBA;
	spec.textureType = GL_TEXTURE_2D;
	spec.type = GL_UNSIGNED_BYTE;

	m_texture = std::make_unique<TextureGL>(spec);
	m_texture->TextureSubImage(data);
}

void Renderer::InitTransforms()
{
	std::random_device rnd;
	std::mt19937_64 gen(rnd());
	std::uniform_real_distribution<float> dist(-1, 1);

	for (int i = 0; i < count; i++)
	{
		TransformComponent component;
		component.translate = glm::vec3(dist(gen) * 100, dist(gen) * 100, dist(gen) * 100);
		component.rotate = glm::vec3(dist(gen) * 180, dist(gen) * 180, dist(gen) * 180);
		component.scale = glm::vec3((dist(gen) + 1) * 2, (dist(gen) + 1) * 2, (dist(gen) + 1) * 2);

		auto entity = m_Registry.CreateEntity();
		m_Registry.AddComponent<TransformComponent>(entity, component);
	}
	
	m_TransformSsbo = std::make_shared<ShaderStorageBufferGL>();
	m_TransformSsbo->BufferStorage(count * sizeof(TransformGLSL), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
}