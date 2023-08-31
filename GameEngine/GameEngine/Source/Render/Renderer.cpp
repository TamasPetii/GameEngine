#include "Renderer.h"

Renderer::Renderer()
{
	InitPhysX();
	CreateCubeRigidBody();

	mPointSize = 5;
	mLineSize = 2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);

	mCamera = new Camera(1200, 800);
	mSceneFrameBuffer = new FrameBufferObject<FBO_ColorTexture>();
	mShadowFrameBuffer = new FrameBufferObject<FBO_DepthTexture>();
	mItemPickFrameBuffer = new FrameBufferObject<FBO_IntegerTexture>();

	mSceneProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Shader/Shader.vert"),
			Shader(GL_GEOMETRY_SHADER, "Source/Shader/Shader.geom"),
			Shader(GL_FRAGMENT_SHADER, "Source/Shader/Shader.frag")
		},
		{
			ShaderLayout(0, "vert_position"),
			ShaderLayout(1, "vert_normal"),
			ShaderLayout(2, "vert_texture")
		}
	);

	mOutlineProgram = new Program(
		{ 
			Shader(GL_VERTEX_SHADER, "Source/Shader/Outline.vert"),
			Shader(GL_FRAGMENT_SHADER, "Source/Shader/Outline.frag")
		}, 
		{
			ShaderLayout(0, "vert_position")
		}
	);

	mItemPickProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Shader/ItemPick.vert"),
			Shader(GL_FRAGMENT_SHADER, "Source/Shader/ItemPick.frag")
		},
		{
			ShaderLayout(0, "vert_position")
		}
	);

	mWireframeProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Shader/Wireframe.vert"),
			Shader(GL_FRAGMENT_SHADER, "Source/Shader/Wireframe.frag")
		},
		{
			ShaderLayout(0, "vert_position"),
			ShaderLayout(1, "vert_normal")
		}
		);

	mNormalsProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Shader/Normals.vert"),
			Shader(GL_GEOMETRY_SHADER, "Source/Shader/Normals.geom"),
			Shader(GL_FRAGMENT_SHADER, "Source/Shader/Normals.frag")
		},
		{
			ShaderLayout(0, "vert_position"),
			ShaderLayout(1, "vert_normal")
		}
		);

	mShadowProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Shader/Shadow.vert"),
			Shader(GL_FRAGMENT_SHADER, "Source/Shader/Shadow.frag")
		},
		{
			ShaderLayout(0, "vert_position")
		}
		);

	mGridProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Shader/Grid.vert"),
			Shader(GL_FRAGMENT_SHADER, "Source/Shader/Grid.frag")
		},
		{
		}
	);

	mTestProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Shader/Test.vert"),
			Shader(GL_FRAGMENT_SHADER, "Source/Shader/Test.frag")
		},
		{
			ShaderLayout(0, "vert_position"),
			ShaderLayout(1, "vert_normal"),
			ShaderLayout(2, "vert_texture")
		}
		);

	mSkyboxProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Shader/Skybox.vert"),
			Shader(GL_FRAGMENT_SHADER, "Source/Shader/Skybox.frag")
		},
		{
			ShaderLayout(0, "vert_position")
		}
		);

	mSkysphereProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Shader/Skysphere.vert"),
			Shader(GL_FRAGMENT_SHADER, "Source/Shader/Skysphere.frag")
		},
		{
			ShaderLayout(0, "vert_position"),
			ShaderLayout(1, "vert_normal"),
			ShaderLayout(2, "vert_texture")
		}
	);
	CreateStartScene();
}

/*
void Renderer::RenderGrid()
{
	mSceneFrameBuffer->Bind();
	mGridProgram->Bind();
	mGridProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	mGrid->Render();
	mSceneFrameBuffer->UnBind();
	mGridProgram->UnBind();
}
*/

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
	entity->GetComponent<LightComponent>()->AttachLight(new DirectionLight);
	entity->GetComponent<MeshComponent>()->AttachMesh(new Shape<Sphere>);
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
		entity->GetComponent<MeshComponent>()->AttachMesh(new Shape<Cube>);
		shapes->AttachChild(entity);

		entity = new Entity();
		entity->Ref_Name() = "Cylinder";
		entity->AddComponent(new TransformComponent);
		entity->AddComponent(new MeshComponent);
		entity->GetComponent<TransformComponent>()->Ref_Translation() = glm::vec3(-2, 0, 0);
		entity->GetComponent<MeshComponent>()->AttachMesh(new Shape<Cylinder>);
		shapes->AttachChild(entity);

		entity = new Entity();
		entity->Ref_Name() = "Torus";
		entity->AddComponent(new TransformComponent);
		entity->AddComponent(new MeshComponent);
		entity->GetComponent<TransformComponent>()->Ref_Translation() = glm::vec3(0, 2, 0);
		entity->GetComponent<MeshComponent>()->AttachMesh(new Shape<Torus>);
		shapes->AttachChild(entity);
	}


	entity = new Entity();
	entity->Ref_Name() = "Ground";
	entity->AddComponent(new TransformComponent);
	entity->AddComponent(new MeshComponent);
	entity->GetComponent<TransformComponent>()->Ref_Translation() = glm::vec3(0, -5, 0);
	entity->GetComponent<TransformComponent>()->Ref_Scale() = glm::vec3(25, 1, 25);
	entity->GetComponent<MeshComponent>()->AttachMesh(new Shape<Cube>);
	m_Scene->AttachEntity(entity);

	entity = new Entity();
	entity->Ref_Name() = "Sky";
	entity->AddComponent(new TransformComponent);
	entity->AddComponent(new MeshComponent);
	entity->AddComponent(new SkyComponent);
	entity->GetComponent<TransformComponent>()->Ref_Translation() = glm::vec3(0, -5, 0);
	entity->GetComponent<TransformComponent>()->Ref_Scale() = glm::vec3(25, 1, 25);
	entity->GetComponent<MeshComponent>()->AttachMesh(new Shape<Cube>);
	entity->GetComponent<SkyComponent>()->Get_SkyType() = SkyType::SkyBox;
	entity->GetComponent<SkyComponent>()->Get_SkyTexture() = ImageTexture::LoadImageMap(paths);
	m_Scene->AttachEntity(entity);

	for (int i = 0; i < 500 && false; i++)
	{
		entity = new Entity();
		entity->Ref_Name() = "Shape" + std::to_string(i);
		entity->AddComponent(new TransformComponent);
		entity->AddComponent(new MeshComponent);
		entity->GetComponent<TransformComponent>()->Ref_Translation() = glm::vec3((i / 50) * 2 , 10, (i % 50) * 2);
		entity->GetComponent<TransformComponent>()->Ref_Scale() = glm::vec3(0.5, 0.5, 0.5);
		entity->GetComponent<MeshComponent>()->AttachMesh(new Shape<Cube>);
		m_Scene->AttachEntity(entity);
	}
}

Renderer::~Renderer()
{
	delete m_Scene;
	delete mCamera;
	delete mSceneFrameBuffer;
	delete mShadowFrameBuffer;
	delete mItemPickFrameBuffer;
	delete mOutlineProgram;
	delete mSceneProgram;
	delete mItemPickProgram;
	delete mWireframeProgram;
	delete mNormalsProgram;
	delete mShadowProgram;
	delete mTestProgram;
	delete mGridProgram;
}


void Renderer::Render()
{
	Renderer::PreRender();
	Renderer::RenderShadowMap(mShadowFrameBuffer, mShadowProgram);
	UploadLightsToShader(mSceneProgram);
	Renderer::RenderItemPick();
	Renderer::RenderScene(mSceneFrameBuffer, mSceneProgram);
	//Renderer::RenderActiveObject(mSceneFrameBuffer, mSceneProgram);
	//Renderer::RenderGrid();
	Renderer::RenderSkyBox(mSceneFrameBuffer, mSkyboxProgram);
	Renderer::PostRender();
}

void Renderer::Update()
{
	static int nbFrames = 0;
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();

	m_Scene->OnUpdate();

	/*
	for (auto entity : mToDeleteEntities)
	{
		std::cout << "Deleted Entity in Renderer" << std::endl;

		if (entity == mActiveEntity) mActiveEntity = nullptr;
		mEntities.erase(entity);
		delete entity;
	}

	for (auto entity : mToEraseEntities)
	{
		mEntities.erase(entity);
	}

	mToEraseEntities.clear();
	mToDeleteEntities.clear();
	*/

	/*

	// Simulate the physics scene
	pxScene->simulate(currentTime - lastTime);
	pxScene->fetchResults(true);

	for (auto entity : mEntities)
	{
		if (rigidSphere && entity->GetId() == 1)
		{
			physx::PxTransform transform = rigidSphere->getGlobalPose();
			float angle;
			physx::PxVec3 axis;
			transform.q.toRadiansAndUnitAxis(angle, axis);
			angle = glm::degrees(angle);


			glm::vec3 rotate = glm::vec3(axis.x * angle, axis.y * angle, axis.z * angle);
			glm::vec3 translate = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
			static glm::vec3 lastTranslate = translate;
			entity->GetComponent<TransformComponent>()->Ref_Rotation() = rotate;
			entity->GetComponent<TransformComponent>()->Ref_Translation() -= lastTranslate - translate;
			lastTranslate = translate;
		}
		if (rigidCube && entity->GetId() == 2)
		{
			physx::PxTransform transform = rigidCube->getGlobalPose();
			float angle;
			physx::PxVec3 axis;
			transform.q.toRadiansAndUnitAxis(angle, axis);
			angle = glm::degrees(angle);

			glm::vec3 rotate = glm::vec3(axis.x * angle, axis.y * angle, axis.z * angle);
			glm::vec3 translate = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
			static glm::vec3 lastTranslate = translate;
			entity->GetComponent<TransformComponent>()->Ref_Rotation() = rotate;
			entity->GetComponent<TransformComponent>()->Ref_Translation() -= lastTranslate - translate;
			lastTranslate = translate;
		}
		else
		{
			if (entity->HasComponent<Rigidbody>())
			{
				physx::PxTransform transform = entity->GetComponent<Rigidbody>()->mRigidBody->getGlobalPose();
				float angle;
				physx::PxVec3 axis;
				transform.q.toRadiansAndUnitAxis(angle, axis);
				angle = glm::degrees(angle);

				glm::vec3 rotate = glm::vec3(axis.x * angle, axis.y * angle, axis.z * angle);
				glm::vec3 translate = glm::vec3(transform.p.x, transform.p.y, transform.p.z);
				entity->GetComponent<TransformComponent>()->Ref_Rotation() = rotate;
				entity->GetComponent<TransformComponent>()->Ref_Translation() = translate;
			}
		}
	}
	*/

	lastTime = currentTime;
}

void Renderer::PreRender()
{
	mSceneFrameBuffer->ClearBuffers();
	mItemPickFrameBuffer->ClearBuffers();
	mShadowFrameBuffer->ClearBuffers();

	dynamic_cast<FrameBufferObject<FBO_IntegerTexture>*>(mItemPickFrameBuffer)->ReadPixelData(0, 0);

	glStencilMask(0x00);
	glPointSize(mPointSize);
	glLineWidth(mLineSize);
}

void Renderer::PostRender()
{

}

void Renderer::uploadToItemPickShader(Entity* entity, Program* shaderProgram)
{
	auto transformComponent = entity->GetComponent<TransformComponent>();
	auto transform = entity->Get_ParentTransformMatrix() * transformComponent->Get_TransformMatrix();

	mItemPickProgram->SetUniform("u_M", transform);
	mItemPickProgram->SetUniform("u_Id", entity->Get_Id());
}

void Renderer::RenderItemPick()
{
	mItemPickFrameBuffer->Bind();
	mItemPickProgram->Bind();
	mItemPickProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());

	auto shaderFunction = std::bind(&Renderer::uploadToItemPickShader, this, std::placeholders::_1, std::placeholders::_2);
	for (auto entity : m_Scene->Get_EntityList())
	{
		RenderEntity(entity, mItemPickProgram, shaderFunction);
	}

	mItemPickProgram->UnBind();
	mItemPickFrameBuffer->UnBind();
}

void Renderer::uploadToMainShader(Entity* entity, Program* shaderProgram)
{
	auto meshComponent = entity->GetComponent<MeshComponent>();
	auto transformComponent = entity->GetComponent<TransformComponent>();
	auto transform = entity->Get_ParentTransformMatrix() * transformComponent->Get_TransformMatrix();

	shaderProgram->SetUniform("u_M", transform);
	shaderProgram->SetUniform("u_MIT", glm::transpose(glm::inverse(transform)));
	shaderProgram->SetUniform("u_Material.ambient", meshComponent->Get_Material().ambient);
	shaderProgram->SetUniform("u_Material.diffuse", meshComponent->Get_Material().diffuse);
	shaderProgram->SetUniform("u_Material.specular", meshComponent->Get_Material().specular);
	shaderProgram->SetUniform("u_Textures.scale", meshComponent->Get_Textures().scale);
	shaderProgram->SetUniform("normalMode", meshComponent->Get_Textures().normal == nullptr ? 0 : 2);
	shaderProgram->SetUniform("u_Textures.useMain", (int)meshComponent->Get_Textures().texture);
	shaderProgram->SetUniform("u_Textures.useNormal", (int)meshComponent->Get_Textures().normal);
	shaderProgram->SetUniform("u_Textures.useHeight", (int)meshComponent->Get_Textures().height);
	shaderProgram->SetUniformTexture("u_Textures.main", 0, meshComponent->Get_Textures().texture);
	shaderProgram->SetUniformTexture("u_Textures.normal", 1, meshComponent->Get_Textures().normal);
	shaderProgram->SetUniformTexture("u_Textures.height", 2, meshComponent->Get_Textures().height);
}

void Renderer::uploadToShadowShader(Entity* entity, Program* shaderProgram)
{
	auto transformComponent = entity->GetComponent<TransformComponent>();
	shaderProgram->SetUniform("u_M", transformComponent->Get_TransformMatrix());
}

void Renderer::RenderEntity(Entity* entity, Program* shaderProgram, std::function<void(Entity*, Program*)> uploadToShader)
{
	if (entity->HasComponent<MeshComponent>() && entity->HasComponent<TransformComponent>())
	{
		uploadToShader(entity, shaderProgram);
		entity->GetComponent<MeshComponent>()->Render();
	}

	for (auto child : entity->Get_Children())
	{
		RenderEntity(child, shaderProgram, uploadToShader);
	}
}

void Renderer::RenderScene(IFrameBufferObject* frameBuffer, Program* shaderProgram)
{
	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_CameraEye", mCamera->GetCameraEye());
	shaderProgram->SetUniform("heightScale", heightScale);

	auto shaderFunction = std::bind(&Renderer::uploadToMainShader, this, std::placeholders::_1, std::placeholders::_2);
	for (auto entity : m_Scene->Get_EntityList())
	{
		RenderEntity(entity, shaderProgram, shaderFunction);
	}

	shaderProgram->UnBind();
	frameBuffer->UnBind();
}

/*
void Renderer::RenderActiveObject(IFrameBufferObject* frameBuffer, Program* shaderProgram)
{

	if (mActiveEntity == nullptr) return;
	if (!mActiveEntity->HasComponent<MeshComponent>()) return;

	TransformComponent* transform = mActiveEntity->GetComponent<TransformComponent>();
	MeshComponent* mesh = mActiveEntity->GetComponent<MeshComponent>();

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_CameraEye", mCamera->GetCameraEye());
	shaderProgram->SetUniform("heightScale", heightScale);



	shaderProgram->UnBind();
	frameBuffer->UnBind();

	//if (mRenderWireframePoints) Renderer::RenderActiveObjectWireframe(frameBuffer, mWireframeProgram, WireframeMode::POINTS);
	//if (mRenderWireframeLines) Renderer::RenderActiveObjectWireframe(frameBuffer, mWireframeProgram, WireframeMode::LINES);
	//if (mRenderWireframeNormals) Renderer::RenderActiveObjectNormals(frameBuffer, mNormalsProgram);
	Renderer::RenderActiveObjectOutline(frameBuffer, mOutlineProgram);
}
*/

/*
void Renderer::RenderActiveObjectWireframe(IFrameBufferObject* frameBuffer, Program* shaderProgram, WireframeMode mode)
{
	if (mActiveEntity == nullptr) return;
	TransformComponent* transform = mActiveEntity->GetComponent<TransformComponent>();
	MeshComponent* mesh = mActiveEntity->GetComponent<MeshComponent>();

	glDisable(GL_CULL_FACE);
	if (mode == WireframeMode::POINTS) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	if (mode == WireframeMode::LINES) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	frameBuffer->Bind();
	shaderProgram->Bind();
	if (mode == WireframeMode::POINTS) shaderProgram->SetUniform("u_Color", mWireframePointsColor);
	if (mode == WireframeMode::LINES) shaderProgram->SetUniform("u_Color", mWireframeLinesColor);
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_M", transform->Get_TransformMatrix());
	shaderProgram->SetUniform("u_MIT", glm::transpose(glm::inverse(transform->Get_TransformMatrix())));
	if (mesh) mesh->Render();
	shaderProgram->UnBind();
	frameBuffer->UnBind();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
*/

void Renderer::RenderActiveObjectOutline(IFrameBufferObject* frameBuffer, Program* shaderProgram)
{
	/*
	if (mActiveEntity == nullptr) return;
	TransformComponent* transform = mActiveEntity->GetComponent<TransformComponent>();
	MeshComponent* mesh = mActiveEntity->GetComponent<MeshComponent>();

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);

	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_M", transform->Get_TransformMatrix() * glm::scale(glm::vec3(1.05)));
	shaderProgram->SetUniform("u_OutlineColor", glm::vec3(1, 0.5, 0));
	if(mesh) mesh->Render();
	shaderProgram->UnBind();
	frameBuffer->UnBind();

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
	*/
}

/*
void Renderer::RenderActiveObjectNormals(IFrameBufferObject* frameBuffer, Program* shaderProgram)
{
	if (mActiveEntity == nullptr) return;
	TransformComponent* transform = mActiveEntity->GetComponent<TransformComponent>();
	MeshComponent* mesh = mActiveEntity->GetComponent<MeshComponent>();

	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_Color", mWireframeNormalsColor);
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_M", transform->Get_TransformMatrix());
	shaderProgram->SetUniform("u_MIT", glm::transpose(glm::inverse(transform->Get_TransformMatrix())));
	if (mesh) mesh->Render();
	shaderProgram->UnBind();
	frameBuffer->UnBind();
}
*/

void Renderer::UploadLightsToShader(Program* shaderProgram)
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
					shaderProgram->SetUniformTexture("u_ShadowMap", 4, mShadowFrameBuffer->GetTextureId());
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

void Renderer::RenderShadowMap(IFrameBufferObject* frameBuffer, Program* shaderProgram)
{
	frameBuffer->Bind();
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
	frameBuffer->UnBind();
}








































void Renderer::InitPhysX()
{
	pxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, pxAllocator, pxErrorCallback);

	isDebugging = true;
	if (isDebugging && pxFoundation)
	{
		pxVisualDebugger = physx::PxCreatePvd(*pxFoundation);
		pxVisualDebugTransport = physx::PxDefaultPvdSocketTransportCreate(VISUAL_DEBUG_HOST.c_str(),
			VISUAL_DEBUG_PORT, VISUAL_DEBUG_TIMEOUT);

		pxVisualDebugger->connect(*pxVisualDebugTransport, physx::PxPvdInstrumentationFlag::eALL);

		pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *pxFoundation, physx::PxTolerancesScale(), true, pxVisualDebugger);
	}
	else
	{
		pxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *pxFoundation, physx::PxTolerancesScale());
	}

	if (!pxFoundation || !pxPhysics)
	{
		std::cerr << "[ERROR] [PHYSICS] :: An error occurred while initializing PhysX";
		return;
	}

	pxDispatcher = physx::PxDefaultCpuDispatcherCreate(4);

	physx::PxSceneDesc sceneDesc(pxPhysics->getTolerancesScale());
	sceneDesc.cpuDispatcher = pxDispatcher;
	sceneDesc.gravity = physx::PxVec3(0.0f, GRAVITY, 0.0f);
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	pxScene = pxPhysics->createScene(sceneDesc);

	if (isDebugging)
	{
		physx::PxPvdSceneClient* visualDebugClient = pxScene->getScenePvdClient();

		if (!visualDebugClient) return;

		visualDebugClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		visualDebugClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		visualDebugClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	//Rigidbody::AttachPhysics(pxPhysics);
	//Rigidbody::AttachScene(pxScene);
}

	
void Renderer::CreateCubeRigidBody()
{
	/*
	physx::PxTransform sphereTransform(0, 20, 0);
	physx::PxSphereGeometry sphereGeometry(1);
	physx::PxShape* sphere = pxPhysics->createShape(sphereGeometry, *pxMaterial);
	rigidSphere = pxPhysics->createRigidDynamic(sphereTransform);
	rigidSphere->attachShape(*sphere);
	//physx::PxRigidBodyExt::updateMassAndInertia(*rigidSphere, CUBE_DENSITY);
	pxScene->addActor(*rigidSphere);

	physx::PxTransform cubeTransform(0, 10, 0);
	physx::PxBoxGeometry cubeGeometry(1, 1, 1);
	physx::PxShape* cube = pxPhysics->createShape(cubeGeometry, *pxMaterial);
	rigidCube = pxPhysics->createRigidDynamic(cubeTransform);
	rigidCube->attachShape(*cube);
	//physx::PxRigidBodyExt::updateMassAndInertia(*rigidCube, CUBE_DENSITY);
	pxScene->addActor(*rigidCube);
	*/

	// Create the ground plane
	physx::PxTransform groundTransform(0, -5, 0);
	physx::PxBoxGeometry groundGeometry(250, 1, 250);  // Adjust the dimensions as needed
	physx::PxShape* groundShape = pxPhysics->createShape(groundGeometry, *pxPhysics->createMaterial(1, 1, 1));
	groundRigidBody = pxPhysics->createRigidStatic(groundTransform);
	groundRigidBody->attachShape(*groundShape);
	pxScene->addActor(*groundRigidBody);
}


void Renderer::RenderSkyBox(IFrameBufferObject* frameBuffer, Program* shaderProgram)
{
	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_M", glm::translate(mCamera->GetCameraEye()));

	for (auto& entity : m_Scene->Get_EntityList())
	{
		if (entity->HasComponent<SkyComponent>() && entity->HasComponent<MeshComponent>())
		{
			auto sky = entity->GetComponent<SkyComponent>();
			auto mesh = entity->GetComponent<MeshComponent>();

			glCullFace(GL_FRONT);
			GLint prevDepthFnc;
			glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);
			glDepthFunc(GL_LEQUAL);

			shaderProgram->SetUniformTexture("uSkyboxTexture", 0, sky->Get_SkyTexture());
			mesh->Render();

			glDepthFunc(prevDepthFnc);
			glCullFace(GL_BACK);
		}
	}

	shaderProgram->UnBind();
	frameBuffer->UnBind();
}