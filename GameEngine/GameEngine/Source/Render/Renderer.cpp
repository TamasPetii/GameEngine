#include "Renderer.h"

Renderer::Renderer()
{
	glLineWidth(2);
	glPointSize(10);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);

	m_Camera = new Camera(1024, 1024);

	#pragma region FrameBufferInitialization

	//Initialize Scene FrameBuffer
	{
		m_FrameBuffersObjects["scene"] = new OpenGL::Classic::FrameBufferObject();

		//Main Texture
		auto mainTextureInfo = OpenGL::FboTextureInfo(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_COLOR_ATTACHMENT0);
	
		//Item Pick Texture
		auto pickTextureInfo = OpenGL::FboTextureInfo(GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT, GL_COLOR_ATTACHMENT1);
		
		pickTextureInfo.AttachClearCallback(
			[](GLuint textureId) -> void
			{
				unsigned int value = 0;
				glClearTexImage(textureId, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &value);
			}
		);
		
		pickTextureInfo.AttachReadCallback(
			[](GLint x, GLint y) -> std::any
			{
				glReadBuffer(GL_COLOR_ATTACHMENT1);
				unsigned int pixelData;
				glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &pixelData);
				return std::make_any<unsigned int>(pixelData);
			}
		);

		//Main render buffer
		auto mainRenderBufferInfo = OpenGL::FboRenderBufferInfo(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
		
		m_FrameBuffersObjects["scene"]->AttachTexture("main", mainTextureInfo);
		m_FrameBuffersObjects["scene"]->AttachTexture("pick", pickTextureInfo);
		m_FrameBuffersObjects["scene"]->AttachRenderBuffer("main", mainRenderBufferInfo);
	}

	//Initialize Shadow FrameBuffer
	{
		m_FrameBuffersObjects["shadow"] = new OpenGL::Classic::FrameBufferObject();

		//Main Texture
		auto shadowTextureInfo = OpenGL::FboTextureInfo();
		shadowTextureInfo.internalFormat = GL_DEPTH_COMPONENT;
		shadowTextureInfo.format = GL_DEPTH_COMPONENT;
		shadowTextureInfo.type = GL_FLOAT;
		shadowTextureInfo.attachment = GL_DEPTH_ATTACHMENT;
		m_FrameBuffersObjects["shadow"]->AttachTexture("shadow", shadowTextureInfo);
		m_FrameBuffersObjects["shadow"]->Resize(2048, 2048);
	}

	#pragma endregion

	#pragma region ProgramInitilaization

	//Initialize Scene Program
	{
		m_ProgramObjects["scene"] = new OpenGL::ProgramObject(
			{
				OpenGL::ShaderObject(GL_VERTEX_SHADER, "Source/Shader/Shader.vert"),
				OpenGL::ShaderObject(GL_GEOMETRY_SHADER, "Source/Shader/Shader.geom"),
				OpenGL::ShaderObject(GL_FRAGMENT_SHADER, "Source/Shader/Shader.frag")
			},
			{
				OpenGL::ShaderObjectInfo(0, "vert_position"),
				OpenGL::ShaderObjectInfo(1, "vert_normal"),
				OpenGL::ShaderObjectInfo(2, "vert_tangent"),
				OpenGL::ShaderObjectInfo(3, "vert_bitangent"),
				OpenGL::ShaderObjectInfo(4, "vert_texture")
			}
		);
		
	}

	//Initialize Normal Program
	{
		m_ProgramObjects["normals"] = new OpenGL::ProgramObject(
			{
				OpenGL::ShaderObject(GL_VERTEX_SHADER,  "Source/Shader/Normals.vert"),
				OpenGL::ShaderObject(GL_GEOMETRY_SHADER, "Source/Shader/Normals.geom"),
				OpenGL::ShaderObject(GL_FRAGMENT_SHADER, "Source/Shader/Normals.frag")
			},
			{
				OpenGL::ShaderObjectInfo(0, "vert_position"),
				OpenGL::ShaderObjectInfo(1, "vert_normal"),
			}
		);
	}

	//Initialize Shadow Program
	{
		m_ProgramObjects["shadow"] = new OpenGL::ProgramObject(
			{
				OpenGL::ShaderObject(GL_VERTEX_SHADER,  "Source/Shader/Shadow.vert"),
				OpenGL::ShaderObject(GL_FRAGMENT_SHADER, "Source/Shader/Shadow.frag")
			},
			{
				OpenGL::ShaderObjectInfo(0, "vert_position")
			}
		);
	}


	//Initialize Outline program
	{
		m_ProgramObjects["outline"] = new OpenGL::ProgramObject(
			{
				OpenGL::ShaderObject(GL_VERTEX_SHADER,  "Source/Shader/Outline.vert"),
				OpenGL::ShaderObject(GL_FRAGMENT_SHADER, "Source/Shader/Outline.frag")
			},
			{
				OpenGL::ShaderObjectInfo(0, "vert_position")
			}
		);
	}

	//Initialize Wireframe Program
	{
		m_ProgramObjects["wireframe"] = new OpenGL::ProgramObject(
			{
				OpenGL::ShaderObject(GL_VERTEX_SHADER,  "Source/Shader/Wireframe.vert"),
				OpenGL::ShaderObject(GL_FRAGMENT_SHADER, "Source/Shader/Wireframe.frag")
			},
			{
				OpenGL::ShaderObjectInfo(0, "vert_position"),
				OpenGL::ShaderObjectInfo(1, "vert_normal")
			}
		);
	}

	//Initialize Skybox Program
	{
		m_ProgramObjects["skybox"] = new OpenGL::ProgramObject(
			{
				OpenGL::ShaderObject(GL_VERTEX_SHADER,  "Source/Shader/Skybox.vert"),
				OpenGL::ShaderObject(GL_FRAGMENT_SHADER, "Source/Shader/Skybox.frag")
			},
			{
				OpenGL::ShaderObjectInfo(0, "vert_position")
			}
		);
	}

	//Initialize Skysphere Program
	{
		m_ProgramObjects["skysphere"] = new OpenGL::ProgramObject(
			{
				OpenGL::ShaderObject(GL_VERTEX_SHADER,  "Source/Shader/Skysphere.vert"),
				OpenGL::ShaderObject(GL_FRAGMENT_SHADER, "Source/Shader/Skysphere.frag")
			},
			{
				OpenGL::ShaderObjectInfo(0, "vert_position"),
				OpenGL::ShaderObjectInfo(1, "vert_normal"),
				OpenGL::ShaderObjectInfo(2, "vert_tangent"),
				OpenGL::ShaderObjectInfo(3, "vert_bitangent"),
				OpenGL::ShaderObjectInfo(4, "vert_texture")
			}
		);
	}

	//Initialize Skybox Program
	{
		m_ProgramObjects["grid"] = new OpenGL::ProgramObject(
			{
				OpenGL::ShaderObject(GL_VERTEX_SHADER,  "Source/Shader/Gridplane.vert"),
				OpenGL::ShaderObject(GL_FRAGMENT_SHADER, "Source/Shader/Gridplane.frag")
			},
			{
				OpenGL::ShaderObjectInfo(0, "vert_position")
			}
		);
	}

	#pragma endregion

	CreateStartScene();
}


void Renderer::RenderGrid()
{
	glDisable(GL_CULL_FACE);

	m_FrameBuffersObjects["scene"]->Bind();
	m_FrameBuffersObjects["scene"]->ActivateTexture("main");

	m_ProgramObjects["grid"]->Bind();
	m_ProgramObjects["grid"]->SetUniform("u_M", glm::scale(glm::vec3(150, 1, 150)));
	m_ProgramObjects["grid"]->SetUniform("u_VP", m_Camera->GetViewProjMatrix());
	Shape::Instance<Plane>()->Render();
	m_ProgramObjects["grid"]->UnBind();

	m_FrameBuffersObjects["scene"]->DeActivateTextures();
	m_FrameBuffersObjects["scene"]->UnBind();

	glEnable(GL_CULL_FACE);
}


void Renderer::CreateStartScene()
{
	static std::vector<std::string> paths =
	{
		"Assets/Images/Skybox/right.jpg",
		"Assets/Images/Skybox/left.jpg",
		"Assets/Images/Skybox/top.jpg",
		"Assets/Images/Skybox/bottom.jpg",
		"Assets/Images/Skybox/front.jpg",
		"Assets/Images/Skybox/back.jpg"
	};

	m_Scene = new Scene();

	Entity* entity;
	
	entity = new Entity();
	entity->Ref_Name() = "Light Source";
	entity->AddComponent(new TransformComponent);
	entity->AddComponent(new MeshComponent);
	entity->AddComponent(new LightComponent);
	entity->GetComponent<TransformComponent>()->Ref_Scale() = glm::vec3(0.1);
	entity->GetComponent<LightComponent>()->AttachLight(new DirectionLight());
	entity->GetComponent<MeshComponent>()->AttachRenderable(new Sphere());
	m_Scene->AttachEntity(entity);
	
	entity = new Entity();
	entity->Ref_Name() = "Shapes";
	entity->AddComponent(new TransformComponent);
	m_Scene->AttachEntity(entity);
	static auto shapes = entity;

	{
		entity = new Entity();
		entity->Ref_Name() = "Cube";
		entity->AddComponent(new TransformComponent);
		entity->AddComponent(new MeshComponent);
		entity->GetComponent<TransformComponent>()->Ref_Translation() = glm::vec3(2, 0, 0);
		entity->GetComponent<MeshComponent>()->AttachRenderable(new Cube());
		shapes->AttachChild(entity);

		entity = new Entity();
		entity->Ref_Name() = "Cylinder";
		entity->AddComponent(new TransformComponent);
		entity->AddComponent(new MeshComponent);
		entity->GetComponent<TransformComponent>()->Ref_Translation() = glm::vec3(-2, 0, 0);
		entity->GetComponent<MeshComponent>()->AttachRenderable(new Cylinder());
		shapes->AttachChild(entity);

		entity = new Entity();
		entity->Ref_Name() = "Torus";
		entity->AddComponent(new TransformComponent);
		entity->AddComponent(new MeshComponent);
		entity->GetComponent<TransformComponent>()->Ref_Translation() = glm::vec3(0, 2, 0);
		entity->GetComponent<MeshComponent>()->AttachRenderable(new Torus());
		shapes->AttachChild(entity);

		entity = new Entity();
		entity->Ref_Name() = "Plane";
		entity->AddComponent(new TransformComponent);
		entity->AddComponent(new MeshComponent);
		entity->GetComponent<MeshComponent>()->AttachRenderable(new Plane);
		shapes->AttachChild(entity);
	}

	entity = new Entity();
	entity->Ref_Name() = "Ground";
	entity->AddComponent(new TransformComponent);
	entity->AddComponent(new MeshComponent);
	entity->GetComponent<TransformComponent>()->Ref_Translation() = glm::vec3(0, -5, 0);
	entity->GetComponent<TransformComponent>()->Ref_Scale() = glm::vec3(25, 1, 25);
	entity->GetComponent<MeshComponent>()->AttachRenderable(new Cube());
	m_Scene->AttachEntity(entity);

	entity = new Entity();
	entity->Ref_Name() = "Skybox";
	entity->AddComponent(new TransformComponent);
	entity->AddComponent(new SkyComponent);
	entity->GetComponent<SkyComponent>()->Get_SkyType() = SkyType::SkyBox;
	entity->GetComponent<SkyComponent>()->Get_SkyTexture() = ImageTexture::LoadImageMap(paths);
	m_Scene->AttachEntity(entity);

	for (int i = 0; i < 100 && false; i++)
	{
		entity = new Entity();
		entity->Ref_Name() = "Shape" + std::to_string(i);
		entity->AddComponent(new TransformComponent);
		entity->AddComponent(new MeshComponent);
		entity->GetComponent<TransformComponent>()->Ref_Translation() = glm::vec3((i / 50) * 2 , 10, (i % 50) * 2);
		entity->GetComponent<TransformComponent>()->Ref_Scale() = glm::vec3(0.5, 0.5, 0.5);
		entity->GetComponent<MeshComponent>()->AttachRenderable(new Cube());
		m_Scene->AttachEntity(entity);
	}
}

Renderer::~Renderer()
{
	//TODO : DELETE
}


void Renderer::Render()
{
	Renderer::PreRender();
	Renderer::RenderShadowMap(m_FrameBuffersObjects["shadow"], m_ProgramObjects["shadow"]);
	UploadLightsToShader(m_ProgramObjects["scene"]);

	Renderer::RenderScene(m_FrameBuffersObjects["scene"], m_ProgramObjects["scene"]);
	//RenderActiveObjectNormals(m_FrameBuffersObjects["scene"], m_ProgramObjects["normals"]);
	//RenderActiveObjectWireframe(m_FrameBuffersObjects["scene"], m_ProgramObjects["wireframe"], WireframeMode::LINES);
	//RenderActiveObjectWireframe(m_FrameBuffersObjects["scene"], m_ProgramObjects["wireframe"], WireframeMode::POINTS);
	RenderActiveObjectOutline(m_FrameBuffersObjects["scene"], m_ProgramObjects["outline"]);
	Renderer::RenderSkyBox(m_FrameBuffersObjects["scene"]);
	Renderer::RenderGrid();
	Renderer::PostRender();
}

void Renderer::Update()
{
	static int nbFrames = 0;
	static double time = 0;
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();

	nbFrames++;
	time += currentTime - lastTime;
	
	if (time > 1)
	{
		std::cout << "Fps: " << nbFrames << std::endl;

		nbFrames = 0;
		time = 0;
	}
	
	m_Scene->OnUpdate();

	lastTime = currentTime;
}

void Renderer::PreRender()
{
	m_FrameBuffersObjects["scene"]->Clear();
	m_FrameBuffersObjects["shadow"]->Clear();

	glStencilMask(0x00);
	glPointSize(mPointSize);
	glLineWidth(mLineSize);
}

void Renderer::PostRender()
{

}

void Renderer::uploadToMainShader(Entity* entity, OpenGL::ProgramObject* shaderProgram)
{
	auto meshComponent = entity->GetComponent<MeshComponent>();
	auto transformComponent = entity->GetComponent<TransformComponent>();
	auto transform = entity->Get_ParentTransformMatrix() * transformComponent->Get_TransformMatrix();
	
	shaderProgram->SetUniform("u_Id", entity->Get_Id());
	shaderProgram->SetUniform("u_M", transform);
	shaderProgram->SetUniform("u_MIT", glm::transpose(glm::inverse(transform)));
	shaderProgram->SetUniform("u_Material.ambient", meshComponent->Get_Material().ambient);
	shaderProgram->SetUniform("u_Material.diffuse", meshComponent->Get_Material().diffuse);
	shaderProgram->SetUniform("u_Material.specular", meshComponent->Get_Material().specular);
	shaderProgram->SetUniform("u_Textures.scale", meshComponent->Get_Textures().scale);
	shaderProgram->SetUniform("u_Textures.scaleX", meshComponent->Get_Textures().scaleX);
	shaderProgram->SetUniform("u_Textures.scaleY", meshComponent->Get_Textures().scaleY);
	shaderProgram->SetUniform("u_HardSurface", (int)meshComponent->Get_HardSurface());
	shaderProgram->SetUniform("u_Textures.useMain", (int)meshComponent->Get_Textures().texture);
	shaderProgram->SetUniform("u_Textures.useNormal", (int)meshComponent->Get_Textures().normal);
	shaderProgram->SetUniform("u_Textures.useHeight", (int)meshComponent->Get_Textures().height);
	shaderProgram->SetUniformTexture("u_Textures.main", 0, meshComponent->Get_Textures().texture);
	shaderProgram->SetUniformTexture("u_Textures.normal", 1, meshComponent->Get_Textures().normal);
	shaderProgram->SetUniformTexture("u_Textures.height", 2, meshComponent->Get_Textures().height);
}

void Renderer::uploadToShadowShader(Entity* entity, OpenGL::ProgramObject* shaderProgram)
{
	auto transformComponent = entity->GetComponent<TransformComponent>();
	shaderProgram->SetUniform("u_M", transformComponent->Get_TransformMatrix());
}

void Renderer::RenderEntity(Entity* entity, OpenGL::ProgramObject* shaderProgram, std::function<void(Entity*, OpenGL::ProgramObject*)> uploadToShader)
{
	if (entity->HasComponent<MeshComponent>() && entity->HasComponent<TransformComponent>() && !entity->HasComponent<SkyComponent>())
	{
		uploadToShader(entity, shaderProgram);
		entity->GetComponent<MeshComponent>()->Render();
	}

	for (auto child : entity->Get_Children())
	{
		RenderEntity(child, shaderProgram, uploadToShader);
	}
}

void Renderer::RenderScene(OpenGL::IFrameBufferObject* frameBuffer, OpenGL::ProgramObject* shaderProgram)
{
	frameBuffer->Bind();
	frameBuffer->ActivateTexture("main");
	frameBuffer->ActivateTexture("pick");

	shaderProgram->Bind();
	shaderProgram->SetUniform("u_VP", m_Camera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_CameraEye", m_Camera->GetCameraEye());
	shaderProgram->SetUniform("heightScale", heightScale);

	auto shaderFunction = std::bind(&Renderer::uploadToMainShader, this, std::placeholders::_1, std::placeholders::_2);
	for (auto entity : m_Scene->Get_EntityList())
	{
		if (m_Scene->IsActive(entity))
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
		}
		else
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0x00);
		}

		RenderEntity(entity, shaderProgram, shaderFunction);

	}

	shaderProgram->UnBind();
	frameBuffer->DeActivateTextures();
	frameBuffer->UnBind();
}


void Renderer::uploadToWireframeShader(Entity* entity, OpenGL::ProgramObject* shaderProgram)
{
	auto transformComponent = entity->GetComponent<TransformComponent>();
	auto transform = entity->Get_ParentTransformMatrix() * transformComponent->Get_TransformMatrix();
	shaderProgram->SetUniform("u_M", transform);
	shaderProgram->SetUniform("u_MIT", glm::transpose(glm::inverse(transform)));
}

void Renderer::RenderActiveObjectWireframe(OpenGL::Classic::FrameBufferObject* frameBuffer, OpenGL::ProgramObject* shaderProgram, WireframeMode mode)
{
	if (m_Scene->Get_ActiveEntity() == nullptr) return;

	glDisable(GL_CULL_FACE);
	if (mode == WireframeMode::POINTS) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	if (mode == WireframeMode::LINES) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	frameBuffer->Bind();
	frameBuffer->ActivateTexture("main");
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_VP", m_Camera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_Color", glm::vec3(0, 0, 0));

	auto shaderFunction = std::bind(&Renderer::uploadToWireframeShader, this, std::placeholders::_1, std::placeholders::_2);
	RenderEntity(m_Scene->Get_ActiveEntity(), shaderProgram, shaderFunction);

	shaderProgram->UnBind();
	frameBuffer->DeActivateTextures();
	frameBuffer->UnBind();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::uploadToOutlineShader(Entity* entity, OpenGL::ProgramObject* shaderProgram)
{
	auto transformComponent = entity->GetComponent<TransformComponent>();
	auto transform = entity->Get_ParentTransformMatrix() * transformComponent->Get_TransformMatrix();
	shaderProgram->SetUniform("u_M", transform * glm::scale(glm::vec3(1.05)));
}

void Renderer::RenderActiveObjectOutline(OpenGL::IFrameBufferObject* frameBuffer, OpenGL::ProgramObject* shaderProgram)
{
	if (m_Scene->Get_ActiveEntity() == nullptr) return;

	frameBuffer->Bind();
	frameBuffer->ActivateTexture("main");
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_VP", m_Camera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_OutlineColor", glm::vec3(1, 0.5, 0));

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);

	auto shaderFunction = std::bind(&Renderer::uploadToOutlineShader, this, std::placeholders::_1, std::placeholders::_2);
	RenderEntity(m_Scene->Get_ActiveEntity(), shaderProgram, shaderFunction);

	shaderProgram->UnBind();
	frameBuffer->DeActivateTextures();
	frameBuffer->UnBind();

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
}

void Renderer::RenderActiveObjectNormals(OpenGL::IFrameBufferObject* frameBuffer, OpenGL::ProgramObject* shaderProgram)
{
	if (m_Scene->Get_ActiveEntity() == nullptr) return;

	auto mesh = m_Scene->Get_ActiveEntity()->GetComponent<MeshComponent>();
	auto transformComponent = m_Scene->Get_ActiveEntity()->GetComponent<TransformComponent>();
	auto transform = m_Scene->Get_ActiveEntity()->Get_ParentTransformMatrix() * transformComponent->Get_TransformMatrix();

	frameBuffer->Bind();
	frameBuffer->ActivateTexture("main");

	shaderProgram->Bind();
	shaderProgram->SetUniform("u_Color", mWireframeNormalsColor);
	shaderProgram->SetUniform("u_VP", m_Camera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_M", transform);
	shaderProgram->SetUniform("u_MIT", glm::transpose(glm::inverse(transform)));
	if (mesh) mesh->Render();
	shaderProgram->UnBind();

	frameBuffer->DeActivateTextures();
	frameBuffer->UnBind();
}


void Renderer::UploadLightsToShader(OpenGL::ProgramObject* shaderProgram)
{
	int directionLightCount = 0;
	int pointLightCount = 0;

	shaderProgram->Bind();
	shaderProgram->SetUniform("u_CastShadows", 0);

	for (auto entity : m_Scene->Get_EntityList())
	{
		if (entity->HasComponent<LightComponent>())
		{
			const auto& lightSource = dynamic_cast<LightSource*>(entity->GetComponent<LightComponent>()->Get_LightSource());

			if (const auto& directionLight = dynamic_cast<DirectionLight*>(lightSource))
			{		
				if (directionLight->Get_CastShadow())
				{
					auto transform = entity->GetComponent<TransformComponent>();
					glm::mat4 view = glm::lookAt(glm::vec3(0,0,0), directionLight->Get_Direction(), glm::vec3(0.0f, 1.0f, 0.0f));
					glm::mat4 ortho = directionLight->Get_OrthoMatrix();
					shaderProgram->SetUniform("u_ShadowVP", ortho * view);
					shaderProgram->SetUniformTexture("u_ShadowMap", 4, m_FrameBuffersObjects["shadow"]->Get_TextureId("shadow"), GL_TEXTURE_2D);
					shaderProgram->SetUniform("u_CastShadows", 1);
				}
				
				shaderProgram->SetUniform("u_DirectionLights[" + std::to_string(directionLightCount) + "].direction", directionLight->Get_Direction());
				shaderProgram->SetUniform("u_DirectionLights[" + std::to_string(directionLightCount) + "].color", directionLight->Get_Color());
				shaderProgram->SetUniform("u_DirectionLights[" + std::to_string(directionLightCount) + "].diffuse", directionLight->Get_DiffuseIntensity());
				shaderProgram->SetUniform("u_DirectionLights[" + std::to_string(directionLightCount) + "].specular", directionLight->Get_SpecularIntensity());
				directionLightCount++;
			}
			else if (const auto& pointLight = dynamic_cast<PointLight*>(lightSource))
			{
				pointLight->Ref_Position() = entity->GetComponent<TransformComponent>()->Get_Translation();
				shaderProgram->SetUniform("u_PointLights[" + std::to_string(pointLightCount) + "].position", pointLight->Get_Position());
				shaderProgram->SetUniform("u_PointLights[" + std::to_string(pointLightCount) + "].color", pointLight->Get_Color());
				shaderProgram->SetUniform("u_PointLights[" + std::to_string(pointLightCount) + "].diffuse", pointLight->Get_DiffuseIntensity());
				shaderProgram->SetUniform("u_PointLights[" + std::to_string(pointLightCount) + "].specular", pointLight->Get_SpecularIntensity());
				pointLightCount++;
			}

		}
	}

	shaderProgram->SetUniform("u_DirectionLightCount", directionLightCount);
	shaderProgram->SetUniform("u_PointLightCount", pointLightCount);
	shaderProgram->UnBind();
}


void Renderer::RenderShadowMap(OpenGL::IFrameBufferObject* frameBuffer, OpenGL::ProgramObject* shaderProgram)
{
	frameBuffer->Bind();
	frameBuffer->ActivateTexture("shadow");

	shaderProgram->Bind();
	glCullFace(GL_FRONT);

	for (auto entity : m_Scene->Get_EntityList())
	{
		if (entity->HasComponent<LightComponent>() && entity->GetComponent<LightComponent>()->Get_LightSource()->Get_CastShadow())
		{
			if (auto directionLight = dynamic_cast<DirectionLight*>(entity->GetComponent<LightComponent>()->Get_LightSource()))
			{
				auto transform = entity->GetComponent<TransformComponent>();
				glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), directionLight->Get_Direction(), glm::vec3(0.0f, 1.0f, 0.0f));
				glm::mat4 ortho = directionLight->Get_OrthoMatrix();
				shaderProgram->SetUniform("u_VP", ortho * view);
			}
		}
	}

	auto shaderFunction = std::bind(&Renderer::uploadToShadowShader, this, std::placeholders::_1, std::placeholders::_2);
	for (auto entity : m_Scene->Get_EntityList())
	{
		RenderEntity(entity, shaderProgram, shaderFunction);
	}

	glCullFace(GL_BACK);
	shaderProgram->UnBind();

	frameBuffer->DeActivateTextures();
	frameBuffer->UnBind();
}

void Renderer::RenderSkyBox(OpenGL::IFrameBufferObject* frameBuffer)
{
	static OpenGL::ProgramObject* shaderProgram;
	static Shape* shape;

	frameBuffer->Bind();
	frameBuffer->ActivateTexture("main");

	for (auto& entity : m_Scene->Get_EntityList())
	{
		if (entity->HasComponent<SkyComponent>())
		{

			if (entity->GetComponent<SkyComponent>()->Get_SkyType() == SkyType::SkyBox)
			{
				shaderProgram = m_ProgramObjects["skybox"];
				shape = Shape::Instance<Cube>();
			}
			else if (entity->GetComponent<SkyComponent>()->Get_SkyType() == SkyType::SkySphere)
			{
				shaderProgram = m_ProgramObjects["skysphere"];
				shape = Shape::Instance<Sphere>();
			}

			shaderProgram->Bind();
			shaderProgram->SetUniform("u_VP", m_Camera->GetViewProjMatrix());
			shaderProgram->SetUniform("u_M", glm::translate(m_Camera->GetCameraEye()));

			glCullFace(GL_FRONT);
			GLint prevDepthFnc;
			glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);
			glDepthFunc(GL_LEQUAL);

			shaderProgram->SetUniformTexture("uSkyTexture", 0, entity->GetComponent<SkyComponent>()->Get_SkyTexture());
			shape->Render();

			glDepthFunc(prevDepthFnc);
			glCullFace(GL_BACK);

			shaderProgram->UnBind();
		}
	}

	frameBuffer->DeActivateTextures();
	frameBuffer->UnBind();
}