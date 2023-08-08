#include "Renderer.h"

Renderer::Renderer()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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

	mOutlineProgram = new Program(
		{ 
			Shader(GL_VERTEX_SHADER, "Source/Engine/Shader/Outline.vert"),
			Shader(GL_FRAGMENT_SHADER, "Source/Engine/Shader/Outline.frag")
		}, 
		{
			ShaderLayout(0, "vert_position")
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);

	mSceneProgram->Bind();
	mSceneProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
	mSceneProgram->SetUniformTexture("u_Texture", 0, mWoodTexture);

	for (GameObject* gameObject : mGameObjects)
	{
		if (gameObject == mActiveObject) continue;
		mSceneProgram->SetUniform("u_M", gameObject->GetTransformMatrix());
		mSceneProgram->SetUniform("u_MIT", glm::inverse(glm::transpose(gameObject->GetTransformMatrix())));
		gameObject->Render();
	}

	mSceneProgram->UnBind();

	if (mActiveObject != nullptr)
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		mSceneProgram->Bind();
		mSceneProgram->SetUniform("u_M", mActiveObject->GetTransformMatrix());
		mSceneProgram->SetUniform("u_MIT", glm::inverse(glm::transpose(mActiveObject->GetTransformMatrix())));
		mActiveObject->Render();
		mSceneProgram->UnBind();

		glDisable(GL_DEPTH_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);

		mOutlineProgram->Bind();
		mOutlineProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());
		mOutlineProgram->SetUniform("u_OutlineColor", glm::vec3(1, 0.5, 0));

		mOutlineProgram->SetUniform("u_M", mActiveObject->GetTransformMatrix() * glm::scale(glm::vec3(1.05)));
		mActiveObject->Render();

		mOutlineProgram->UnBind();

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);
	}


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