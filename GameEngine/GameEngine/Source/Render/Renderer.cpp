#include "Renderer.h"

Renderer::Renderer()
{
	mPointSize = 5;
	mLineSize = 2;

	glFrontFace(GL_CCW);
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

	mWireframeProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Engine/Shader/Wireframe.vert"),
			Shader(GL_FRAGMENT_SHADER, "Source/Engine/Shader/Wireframe.frag")
		},
		{
			ShaderLayout(0, "vert_position"),
			ShaderLayout(1, "vert_normal")
		}
		);

	mNormalsProgram = new Program(
		{
			Shader(GL_VERTEX_SHADER, "Source/Engine/Shader/Normals.vert"),
			Shader(GL_GEOMETRY_SHADER, "Source/Engine/Shader/Normals.geom"),
			Shader(GL_FRAGMENT_SHADER, "Source/Engine/Shader/Normals.frag")
		},
		{
			ShaderLayout(0, "vert_position"),
			ShaderLayout(1, "vert_normal")
		}
		);

	mWoodTexture = Texture2D::LoadTexture2D("Assets/Wood.jpg");
	mGameObjects.insert(new Torus());
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
	Renderer::RenderScene(mSceneFrameBuffer, mSceneProgram);
	Renderer::RenderActiveObject(mSceneFrameBuffer, mSceneProgram);
	Renderer::PostRender();
}

void Renderer::Update()
{

}

void Renderer::PreRender()
{
	mSceneFrameBuffer->Clear();
	mItemPickFrameBuffer->Clear();

	glStencilMask(0x00);
	glPointSize(mPointSize);
	glLineWidth(mLineSize);
}

void Renderer::PostRender()
{
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

void Renderer::RenderItemPick()
{
	mItemPickFrameBuffer->Bind();
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

void Renderer::RenderScene(FrameBuffer* frameBuffer, Program* shaderProgram)
{
	frameBuffer->Bind();
	shaderProgram->Bind();
	shaderProgram->SetUniform("u_VP", mCamera->GetViewProjMatrix());

	for (GameObject* gameObject : mGameObjects)
	{
		if (gameObject == mActiveObject || !dynamic_cast<Shape*>(gameObject)) continue;
		Shape* shape = dynamic_cast<Shape*>(gameObject);

		shaderProgram->SetUniform("u_M", shape->GetTransformMatrix());
		shaderProgram->SetUniform("u_MIT", glm::inverse(glm::transpose(shape->GetTransformMatrix())));
		shaderProgram->SetUniform("u_UseTexture", (int)shape->GetUseTextureRef());
		shaderProgram->SetUniform("u_Color", shape->GetColorRef());
		shaderProgram->SetUniformTexture("u_Texture", 0, shape->GetTexture());

		gameObject->Render();
	}

	shaderProgram->UnBind();
	frameBuffer->UnBind();
}

void Renderer::RenderActiveObject(FrameBuffer* frameBuffer, Program* shaderProgram)
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

void Renderer::RenderActiveObjectWireframe(FrameBuffer* frameBuffer, Program* shaderProgram, WireframeMode mode)
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

void Renderer::RenderActiveObjectOutline(FrameBuffer* frameBuffer, Program* shaderProgram)
{
	if (mActiveObject == nullptr || dynamic_cast<Shape*>(mActiveObject) == nullptr) return;
	Shape* activeShape = dynamic_cast<Shape*>(mActiveObject);

	glDisable(GL_DEPTH_TEST);
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

void Renderer::RenderActiveObjectNormals(FrameBuffer* frameBuffer, Program* shaderProgram)
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