#include "Renderer.h"

Renderer::Renderer()
{
	mPointSize = 5;
	mLineSize = 2;

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

	CreateStartScene();
}

void Renderer::CreateStartScene()
{
	Entity* entity = new Entity();
	MeshComponent* meshComponent = new MeshComponent();
	meshComponent->AttachMesh(new Shape<Cube>());
	entity->AddComponent(meshComponent);
	entity->AddComponent(new TransformComponent());
	mEntities.insert(entity);

	entity = new Entity();
	TransformComponent* transformComponent = new TransformComponent();
	transformComponent->GetScale() = glm::vec3(15, 0.2, 15);
	meshComponent = new MeshComponent();
	meshComponent->AttachMesh(new Shape<Cube>());
	entity->AddComponent(meshComponent);
	entity->AddComponent<>(transformComponent);
	mEntities.insert(entity);
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
	Texture2D::ClearTextures();
}


void Renderer::Render()
{
	Renderer::PreRender();
	//Renderer::RenderShadowMap(mShadowFrameBuffer, mShadowProgram);
	//Renderer::RenderItemPick();
	Renderer::RenderScene(mSceneFrameBuffer, mTestProgram);
	//Renderer::RenderActiveObject(mSceneFrameBuffer, mSceneProgram);
	Renderer::PostRender();
}

void Renderer::Update()
{

}

void Renderer::PreRender()
{
	mSceneFrameBuffer->ClearBuffers();
	mItemPickFrameBuffer->ClearBuffers();
	mShadowFrameBuffer->ClearBuffers();
	//UploadLightsToShader(mSceneProgram);

	glStencilMask(0x00);
	glPointSize(mPointSize);
	glLineWidth(mLineSize);
}

void Renderer::PostRender()
{
}
/*
bool Renderer::FindActiveObject(int id)
{
	for (auto gameObject : mGameObjects)
	{
		if (gameObject->GetId() == id)
		{
			mActiveObject = gameObject;
			return true;
		}
	}
	return false;
}
*/
/*
void Renderer::RenderItemPick()
{
	mItemPickFrameBuffer->Bind();
	mItemPickProgram->Bind();
	mItemPickProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());

	for (GameObject* gameObject : mGameObjects)
	{
		IMesh* mesh = dynamic_cast<IMesh*>(gameObject);
		if (mesh)
		{
			mItemPickProgram->SetUniform("u_M", gameObject->GetTransformMatrix());
			mItemPickProgram->SetUniform("u_Id", gameObject->GetId());
			gameObject->Render();
		}
	}

	mItemPickProgram->UnBind();
	mItemPickFrameBuffer->UnBind();
}
*/

void Renderer::RenderScene(IFrameBufferObject* frameBuffer, Program* shaderProgram)
{
	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_CameraEye", mCamera->GetCameraEye());

	for (auto entity : mEntities)
	{
		//if (gameObject == mActiveObject || !dynamic_cast<Shape*>(gameObject)) continue;
		//Shape* shape = dynamic_cast<Shape*>(gameObject);

		TransformComponent* transform = entity->GetComponent<TransformComponent>();

		shaderProgram->SetUniform("u_M", transform->GetTransformMatrix());
		shaderProgram->SetUniform("u_MIT", glm::inverse(glm::transpose(transform->GetTransformMatrix())));
		//shaderProgram->SetUniform("u_UseTexture", (int)shape->GetUseTextureRef());
		//shaderProgram->SetUniform("u_Color", shape->GetColorRef());
		//shaderProgram->SetUniformTexture("u_Texture", 0, shape->GetTexture());

		MeshComponent* mesh = entity->GetComponent<MeshComponent>();
		mesh->Render();
	}

	shaderProgram->UnBind();
	frameBuffer->UnBind();
}

/*
void Renderer::RenderActiveObject(IFrameBufferObject* frameBuffer, Program* shaderProgram)
{
	if (mActiveObject == nullptr || dynamic_cast<Shape*>(mActiveObject) == nullptr) return;
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);

	Shape* activeShape = dynamic_cast<Shape*>(mActiveObject);
	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_M", activeShape->GetTransformMatrix());
	shaderProgram->SetUniform("u_MIT", glm::inverse(glm::transpose(activeShape->GetTransformMatrix())));
	shaderProgram->SetUniform("u_UseTexture", (int)activeShape->GetUseTextureRef());
	shaderProgram->SetUniform("u_Color", activeShape->GetColorRef());
	shaderProgram->SetUniformTexture("u_Texture", 0, activeShape->GetTexture());
	activeShape->Render();
	shaderProgram->UnBind();
	frameBuffer->UnBind();

	if (mRenderWireframePoints) Renderer::RenderActiveObjectWireframe(frameBuffer, mWireframeProgram, WireframeMode::POINTS);
	if (mRenderWireframeLines) Renderer::RenderActiveObjectWireframe(frameBuffer, mWireframeProgram, WireframeMode::LINES);
	if (mRenderWireframeNormals) Renderer::RenderActiveObjectNormals(frameBuffer, mNormalsProgram);
	Renderer::RenderActiveObjectOutline(frameBuffer, mOutlineProgram);

}

void Renderer::RenderActiveObjectWireframe(IFrameBufferObject* frameBuffer, Program* shaderProgram, WireframeMode mode)
{
	if (mActiveObject == nullptr || dynamic_cast<Shape*>(mActiveObject) == nullptr) return;
	glDisable(GL_CULL_FACE);
	if (mode == WireframeMode::POINTS) glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	if (mode == WireframeMode::LINES) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Shape* activeShape = dynamic_cast<Shape*>(mActiveObject);

	frameBuffer->Bind();
	shaderProgram->Bind();
	if (mode == WireframeMode::POINTS) shaderProgram->SetUniform("u_Color", mWireframePointsColor);;
	if (mode == WireframeMode::LINES) shaderProgram->SetUniform("u_Color", mWireframeLinesColor);;
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_M", activeShape->GetTransformMatrix());
	shaderProgram->SetUniform("u_MIT", glm::inverse(glm::transpose(activeShape->GetTransformMatrix())));
	activeShape->Render();
	shaderProgram->UnBind();
	frameBuffer->UnBind();

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::RenderActiveObjectOutline(IFrameBufferObject* frameBuffer, Program* shaderProgram)
{
	if (mActiveObject == nullptr || dynamic_cast<Shape*>(mActiveObject) == nullptr) return;
	Shape* activeShape = dynamic_cast<Shape*>(mActiveObject);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);

	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_M", activeShape->GetTransformMatrix() * glm::scale(glm::vec3(1.05)));
	shaderProgram->SetUniform("u_OutlineColor", glm::vec3(1, 0.5, 0));
	activeShape->Render();
	shaderProgram->UnBind();
	frameBuffer->UnBind();

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::RenderActiveObjectNormals(IFrameBufferObject* frameBuffer, Program* shaderProgram)
{
	if (mActiveObject == nullptr || dynamic_cast<Shape*>(mActiveObject) == nullptr) return;
	Shape* activeShape = dynamic_cast<Shape*>(mActiveObject);

	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_Color", mWireframeNormalsColor);
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	shaderProgram->SetUniform("u_M", activeShape->GetTransformMatrix());
	shaderProgram->SetUniform("u_MIT", glm::inverse(glm::transpose(activeShape->GetTransformMatrix())));

	activeShape->Render();
	shaderProgram->UnBind();
	frameBuffer->UnBind();
}
*/

/*
void Renderer::UploadLightsToShader(Program* shaderProgram)
{
	int directionLightCount = 0;
	int pointLightCount = 0;
	DirectionLight* directionLight;
	PointLight* pointLight;

	shaderProgram->Bind();

	for (auto gameObject : mGameObjects)
	{
		if (directionLight = dynamic_cast<DirectionLight*>(gameObject))
		{
			shaderProgram->SetUniform("u_ShadowVP", directionLight->GetViewProjMatrix());
			shaderProgram->SetUniformTexture("u_ShadowMap", 1, mShadowFrameBuffer->GetTextureId());

			shaderProgram->SetUniform("u_DirectionLights[" + std::to_string(directionLightCount) + "].direction", directionLight->GetDirectionRef());
			shaderProgram->SetUniform("u_DirectionLights[" + std::to_string(directionLightCount) + "].color", directionLight->GetColorRef());
			shaderProgram->SetUniform("u_DirectionLights[" + std::to_string(directionLightCount) + "].diffuse", directionLight->GetDiffuseRef());
			shaderProgram->SetUniform("u_DirectionLights[" + std::to_string(directionLightCount) + "].specular", directionLight->GetSpecularRef());
			shaderProgram->SetUniform("u_DirectionLights[" + std::to_string(directionLightCount) + "].volume", directionLight->GetVolumeRef());
			directionLightCount++;
		}
		else if (pointLight = dynamic_cast<PointLight*>(gameObject))
		{
			shaderProgram->SetUniform("u_PointLights[" + std::to_string(pointLightCount) + "].position", pointLight->GetPositionRef());
			shaderProgram->SetUniform("u_PointLights[" + std::to_string(pointLightCount) + "].color", pointLight->GetColorRef());
			shaderProgram->SetUniform("u_PointLights[" + std::to_string(pointLightCount) + "].diffuse", pointLight->GetDiffuseRef());
			shaderProgram->SetUniform("u_PointLights[" + std::to_string(pointLightCount) + "].specular", pointLight->GetSpecularRef());
			shaderProgram->SetUniform("u_PointLights[" + std::to_string(pointLightCount) + "].volume", pointLight->GetVolumeRef());
			pointLightCount++;
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
	glDisable(GL_CULL_FACE);

	for (auto object : mGameObjects)
	{
		DirectionLight* light = dynamic_cast<DirectionLight*>(object);
		if (light == nullptr) continue;

		shaderProgram->SetUniform("u_VP", light->GetViewProjMatrix());
		for (auto object : mGameObjects)
		{
			if (dynamic_cast<Shape*>(object))
			{
				Shape* shape = dynamic_cast<Shape*>(object);
				shaderProgram->SetUniform("u_M", shape->GetTransformMatrix());
				shaderProgram->SetUniform("u_Color", shape->GetColorRef());
				shape->Render();
			}
		}
		break;
	}

	glEnable(GL_CULL_FACE);
	shaderProgram->UnBind();
	frameBuffer->UnBind();
}
*/