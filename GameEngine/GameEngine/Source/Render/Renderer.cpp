#include "Renderer.h"

Renderer::Renderer()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);

	mCamera = new Camera(1200, 800);

	mSceneFrameBuffer = new FrameBuffer(1200, 800, FrameBufferType::ColorBuffer);
	mSceneProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Engine/Shader/Shader.vert"),
			Shader(GL_FRAGMENT_SHADER, "Source/Engine/Shader/Shader.frag")
		},
		{
			ShaderLayout(0, "vert_position"),
			ShaderLayout(1, "vert_normal"),
			ShaderLayout(2, "vert_texture")
		}
	);

	mItemPickFrameBuffer = new FrameBuffer(1200, 800, FrameBufferType::IntegerBuffer);
	mItemPickProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Engine/Shader/ItemPick.vert"),
			Shader(GL_FRAGMENT_SHADER, "Source/Engine/Shader/ItemPick.frag")
		},
		{
			ShaderLayout(0, "vert_position")
		}
	);

	mWoodTexture = Texture2D::LoadTexture2D("Assets/Wood.jpg");
	mGameObjects.insert(new Cube());
}

Renderer::~Renderer()
{
	for (GameObject* gameObject : mGameObjects)
	{
		delete gameObject;
	}
	delete mCamera;
	delete mSceneProgram;
	delete mSceneFrameBuffer;
	Texture2D::ClearTextures();
}

void Renderer::Render()
{
	Renderer::PreRender();
	Renderer::RenderItemPick();
	Renderer::RenderScene();
	Renderer::PostRender();
}

void Renderer::Update()
{

}

void Renderer::PreRender()
{
}

void Renderer::PostRender()
{
}

void Renderer::RenderItemPick()
{
	mItemPickFrameBuffer->Bind();
	int value = -1;
	glClearTexImage(mItemPickFrameBuffer->GetTextureId(), 0, GL_RED_INTEGER, GL_INT, &value);
	glClear(GL_DEPTH_BUFFER_BIT);

	mItemPickProgram->Bind();
	mItemPickProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());

	for (GameObject* gameObject : mGameObjects)
	{
		mItemPickProgram->SetUniform("u_M", gameObject->GetTransformMatrix());
		mItemPickProgram->SetUniform("u_Id", gameObject->GetId());
		gameObject->Render();
	}

	mItemPickProgram->UnBind();
	mItemPickFrameBuffer->UnBind();
}

void Renderer::RenderScene()
{
	mSceneFrameBuffer->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mSceneProgram->Bind();
	mSceneProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	mSceneProgram->SetUniformTexture("u_Texture", 0, mWoodTexture);

	for (GameObject* gameObject : mGameObjects)
	{
		mSceneProgram->SetUniform("u_M", gameObject->GetTransformMatrix());
		mSceneProgram->SetUniform("u_MIT", glm::inverse(glm::transpose(gameObject->GetTransformMatrix())));
		gameObject->Render();
	}

	mSceneProgram->UnBind();
	mSceneFrameBuffer->UnBind();
}

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