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

	Texture2D::LoadTexture2D("Assets/Wood.jpg");
	Texture2D::LoadTexture2D("Assets/DirectionLightIcon.png");
	Texture2D::LoadTexture2D("Assets/PointLightIcon.png");

	mCamera = new Camera(1200, 800);
	mSceneFrameBuffer = new FrameBufferObject<FBO_ColorTexture>();
	mShadowFrameBuffer = new FrameBufferObject<FBO_DepthTexture>();
	mItemPickFrameBuffer = new FrameBufferObject<FBO_IntegerTexture>();

	mSceneProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Shader/Shader.vert"),
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

	mGrid = new Shape<Plane>();
	CreateStartScene();
}

void Renderer::RenderGrid()
{
	mSceneFrameBuffer->Bind();
	mGridProgram->Bind();
	mGridProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	mGrid->Render();
	mSceneFrameBuffer->UnBind();
	mGridProgram->UnBind();
}

void Renderer::CreateStartScene()
{
	Entity* entity;
	MeshComponent* meshComponent;
	LightComponent* lightComponent;
	TransformComponent* transformComponent;

	entity = new Entity();
	lightComponent = new LightComponent();
	lightComponent->AttachLight(new Light<Directional>(glm::vec3(-0.5, -0.8, -0.7)));
	transformComponent = new TransformComponent();
	transformComponent->GetScale() = glm::vec3(0.1, 0.1, 0.1);
	meshComponent = new MeshComponent();
	meshComponent->AttachMesh(new Shape<Sphere>());
	entity->AddComponent(meshComponent);
	entity->AddComponent(transformComponent);
	entity->AddComponent(lightComponent);
	mEntities.insert(entity);

	entity = new Entity();
	meshComponent = new MeshComponent();
	meshComponent->AttachMesh(new Shape<Sphere>());
	transformComponent = new TransformComponent();
	transformComponent->GetTranslation() = glm::vec3(0, 20, 0);
	transformComponent->GetScale() = glm::vec3(1, 1, 1);
	entity->AddComponent(meshComponent);
	entity->AddComponent(transformComponent);
	mEntities.insert(entity);

	entity = new Entity();
	meshComponent = new MeshComponent();
	meshComponent->AttachMesh(new Shape<Cube>());
	transformComponent = new TransformComponent();
	transformComponent->GetTranslation() = glm::vec3(0, 10, 0);
	transformComponent->GetScale() = glm::vec3(1, 1, 1);
	entity->AddComponent(meshComponent);
	entity->AddComponent(transformComponent);
	mEntities.insert(entity);

	entity = new Entity();
	transformComponent = new TransformComponent();
	transformComponent->GetTranslation() = glm::vec3(0, -5, 0);
	transformComponent->GetScale() = glm::vec3(250, 1, 250);
	meshComponent = new MeshComponent();
	meshComponent->AttachMesh(new Shape<Cube>());
	entity->AddComponent(meshComponent);
	entity->AddComponent(transformComponent);
	mEntities.insert(entity);

	for (int i = 0; i < 0; i++)
	{
		Entity* entity = new Entity();
		MeshComponent* meshComponent = new MeshComponent();
		TransformComponent* transformComponent = new TransformComponent();
		Rigidbody* rigidBody = new Rigidbody(physx::PxTransform(0, 10 + i * 2, 0));
		transformComponent->GetTranslation() = glm::vec3(0, 10 + i * 2, 0);
		meshComponent->AttachMesh(new Shape<Cube>());

		entity->AddComponent(rigidBody);
		entity->AddComponent(meshComponent);
		entity->AddComponent(transformComponent);

		mEntities.insert(entity);
	}
}

Renderer::~Renderer()
{
	for (Entity* entity : mEntities)
	{
		delete entity;
	}

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
	Texture2D::ClearTextures();
}


void Renderer::Render()
{
	Renderer::PreRender();
	Renderer::RenderShadowMap(mShadowFrameBuffer, mShadowProgram);
	UploadLightsToShader(mSceneProgram);
	Renderer::RenderItemPick();
	Renderer::RenderScene(mSceneFrameBuffer, mSceneProgram);
	Renderer::RenderActiveObject(mSceneFrameBuffer, mSceneProgram);
	//Renderer::RenderGrid();
	Renderer::PostRender();
}

void Renderer::Update()
{
	static int nbFrames = 0;
	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();

	for (auto entity : mToDeleteEntities)
	{
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
			entity->GetComponent<TransformComponent>()->GetRotation() = rotate;
			entity->GetComponent<TransformComponent>()->GetTranslation() -= lastTranslate - translate;
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
			entity->GetComponent<TransformComponent>()->GetRotation() = rotate;
			entity->GetComponent<TransformComponent>()->GetTranslation() -= lastTranslate - translate;
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
				entity->GetComponent<TransformComponent>()->GetRotation() = rotate;
				entity->GetComponent<TransformComponent>()->GetTranslation() = translate;
			}
		}
	}

	lastTime = currentTime;
}

void Renderer::PreRender()
{
	mSceneFrameBuffer->ClearBuffers();
	mItemPickFrameBuffer->ClearBuffers();
	mShadowFrameBuffer->ClearBuffers();

	glStencilMask(0x00);
	glPointSize(mPointSize);
	glLineWidth(mLineSize);
}

void Renderer::PostRender()
{

}

bool Renderer::FindActiveEntity(int id)
{
	for (auto entity : mEntities)
	{
		if (entity->GetId() == id)
		{
			mActiveEntity = entity;
			return true;
		}
	}
	return false;
}

void Renderer::RenderItemPick()
{
	mItemPickFrameBuffer->Bind();
	mItemPickProgram->Bind();
	mItemPickProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());

	for (auto entity : mEntities)
	{
		if (entity->HasComponent<MeshComponent>())
		{
			TransformComponent* transform = entity->GetComponent<TransformComponent>();
			MeshComponent* mesh = entity->GetComponent<MeshComponent>();
			mItemPickProgram->SetUniform("u_M", transform->GetTransformMatrix());
			mItemPickProgram->SetUniform("u_Id", entity->GetId());
			if(mesh) mesh->Render();
		}
	}

	mItemPickProgram->UnBind();
	mItemPickFrameBuffer->UnBind();
}

void Renderer::RenderScene(IFrameBufferObject* frameBuffer, Program* shaderProgram)
{
	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_CameraEye", mCamera->GetCameraEye());

	for (auto& obj : Entity::mEntities)
	{
		Entity* entity = obj.second;
		if (entity == mActiveEntity || !entity->HasComponent<MeshComponent>()) continue;
		MeshComponent* mesh = entity->GetComponent<MeshComponent>();
		TransformComponent* meshTransform = entity->GetComponent<TransformComponent>();
		glm::mat4 parentTransform = entity->GetParentTransformMatrix();
		glm::mat4 transform = parentTransform * meshTransform->GetTransformMatrix();
		shaderProgram->SetUniform("u_M", transform);
		shaderProgram->SetUniform("u_MIT", glm::transpose(glm::inverse(transform)));
		shaderProgram->SetUniform("u_Color", mesh->GetColor());
		shaderProgram->SetUniform("u_HasTexture", (int)mesh->GetTexture());
		shaderProgram->SetUniform("u_CastShadows", (int)mShadowEntity);
		shaderProgram->SetUniformTexture("u_Texture", 0, mesh->GetTexture());
		mesh->Render();
	}

	shaderProgram->UnBind();
	frameBuffer->UnBind();
}


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
	shaderProgram->SetUniform("u_M", transform->GetTransformMatrix());
	shaderProgram->SetUniform("u_MIT", glm::transpose(glm::inverse(transform->GetTransformMatrix())));
	shaderProgram->SetUniform("u_Color", mesh->GetColor());
	shaderProgram->SetUniform("u_HasTexture", (int)mesh->GetTexture());
	shaderProgram->SetUniformTexture("u_Texture", 0, mesh->GetTexture());
	mesh->Render();
	shaderProgram->UnBind();
	frameBuffer->UnBind();

	if (mRenderWireframePoints) Renderer::RenderActiveObjectWireframe(frameBuffer, mWireframeProgram, WireframeMode::POINTS);
	if (mRenderWireframeLines) Renderer::RenderActiveObjectWireframe(frameBuffer, mWireframeProgram, WireframeMode::LINES);
	if (mRenderWireframeNormals) Renderer::RenderActiveObjectNormals(frameBuffer, mNormalsProgram);
	Renderer::RenderActiveObjectOutline(frameBuffer, mOutlineProgram);
}

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
	shaderProgram->SetUniform("u_M", transform->GetTransformMatrix());
	shaderProgram->SetUniform("u_MIT", glm::transpose(glm::inverse(transform->GetTransformMatrix())));
	if (mesh) mesh->Render();
	shaderProgram->UnBind();
	frameBuffer->UnBind();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::RenderActiveObjectOutline(IFrameBufferObject* frameBuffer, Program* shaderProgram)
{
	if (mActiveEntity == nullptr) return;
	TransformComponent* transform = mActiveEntity->GetComponent<TransformComponent>();
	MeshComponent* mesh = mActiveEntity->GetComponent<MeshComponent>();

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);

	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_M", transform->GetTransformMatrix() * glm::scale(glm::vec3(1.05)));
	shaderProgram->SetUniform("u_OutlineColor", glm::vec3(1, 0.5, 0));
	if(mesh) mesh->Render();
	shaderProgram->UnBind();
	frameBuffer->UnBind();

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::RenderActiveObjectNormals(IFrameBufferObject* frameBuffer, Program* shaderProgram)
{
	if (mActiveEntity == nullptr) return;
	TransformComponent* transform = mActiveEntity->GetComponent<TransformComponent>();
	MeshComponent* mesh = mActiveEntity->GetComponent<MeshComponent>();

	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_Color", mWireframeNormalsColor);
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_M", transform->GetTransformMatrix());
	shaderProgram->SetUniform("u_MIT", glm::transpose(glm::inverse(transform->GetTransformMatrix())));
	if (mesh) mesh->Render();
	shaderProgram->UnBind();
	frameBuffer->UnBind();
}

void Renderer::UploadLightsToShader(Program* shaderProgram)
{
	int directionLightCount = 0;
	int pointLightCount = 0;
	Light<Directional>* directionLight;
	Light<Point>* pointLight;

	shaderProgram->Bind();

	for (auto entity : mEntities)
	{
		if (entity->HasComponent<LightComponent>())
		{
			ILight* light = entity->GetComponent<LightComponent>()->GetLightSource();
			if (directionLight = dynamic_cast<Light<Directional>*>(light))
			{
				if (entity == mShadowEntity)
				{
					TransformComponent* shadowTransform = mShadowEntity->GetComponent<TransformComponent>();
					glm::mat4 view = glm::lookAt(shadowTransform->GetTranslation(), shadowTransform->GetTranslation() + directionLight->GetDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
					glm::mat4 ortho = directionLight->GetOrthoMatrix();
					shaderProgram->SetUniform("u_ShadowVP", ortho * view);
					shaderProgram->SetUniformTexture("u_ShadowMap", 1, mShadowFrameBuffer->GetTextureId());
				}

				shaderProgram->SetUniform("u_DirectionLights[" + std::to_string(directionLightCount) + "].direction", directionLight->GetDirection());
				shaderProgram->SetUniform("u_DirectionLights[" + std::to_string(directionLightCount) + "].color", directionLight->GetColor());
				shaderProgram->SetUniform("u_DirectionLights[" + std::to_string(directionLightCount) + "].diffuse", directionLight->GetDiffuseIntensity());
				shaderProgram->SetUniform("u_DirectionLights[" + std::to_string(directionLightCount) + "].specular", directionLight->GetSpecularIntensity());
				directionLightCount++;
			}
			else if (pointLight = dynamic_cast<Light<Point>*>(light))
			{
				pointLight->GetPosition() = entity->GetComponent<TransformComponent>()->GetTranslation();
				shaderProgram->SetUniform("u_PointLights[" + std::to_string(pointLightCount) + "].position", pointLight->GetPosition());
				shaderProgram->SetUniform("u_PointLights[" + std::to_string(pointLightCount) + "].color", pointLight->GetColor());
				shaderProgram->SetUniform("u_PointLights[" + std::to_string(pointLightCount) + "].diffuse", pointLight->GetDiffuseIntensity());
				shaderProgram->SetUniform("u_PointLights[" + std::to_string(pointLightCount) + "].specular", pointLight->GetSpecularIntensity());
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
	if (mShadowEntity == nullptr) return;
	frameBuffer->Bind();
	shaderProgram->Bind();
	glCullFace(GL_FRONT);

	TransformComponent* shadowTransform = mShadowEntity->GetComponent<TransformComponent>();
	Light<Directional>* shadowLight = dynamic_cast<Light<Directional>*>(mShadowEntity->GetComponent<LightComponent>()->GetLightSource());
	glm::mat4 view = glm::lookAt(shadowTransform->GetTranslation(), shadowTransform->GetTranslation() + shadowLight->GetDirection(), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 ortho = shadowLight->GetOrthoMatrix();
	shaderProgram->SetUniform("u_VP", ortho * view);

	for (auto entity : mEntities)
	{
		if (mShadowEntity == entity || !entity->HasComponent<MeshComponent>()) continue;
		TransformComponent* transform = entity->GetComponent<TransformComponent>();
		MeshComponent* mesh = entity->GetComponent<MeshComponent>();

		shaderProgram->SetUniform("u_M", transform->GetTransformMatrix());
		mesh->Render();
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

	Rigidbody::AttachPhysics(pxPhysics);
	Rigidbody::AttachScene(pxScene);
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