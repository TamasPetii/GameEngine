#pragma once
#include "../Engine/Engine.h"
#include "GameObjects/Shapes/Cube.h"
#include "GameObjects/Shapes/Sphere.h"
#include "GameObjects/Shapes/Torus.h"
#include "GameObjects/Shapes/Cylinder.h"
#include <unordered_set>

enum WireframeMode
{
	POINTS,
	LINES
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render();
	void Update();
	inline FrameBuffer* GetItemPickFrameBuffer() const { return mItemPickFrameBuffer; }
	inline FrameBuffer* GetSceneFrameBuffer() const { return mSceneFrameBuffer; }
	inline Camera* GetCamera() const { return mCamera; }
	inline std::unordered_set<GameObject*>& GetGameObjects() { return mGameObjects; }
	inline GameObject* GetActiveObject() { return mActiveObject; }
	bool FindActiveObject(int id);
	inline void NoActiveObject() { mActiveObject = nullptr; }
	bool mRenderWireframePoints;
	bool mRenderWireframeLines;
private:
	void PreRender();
	void PostRender();
	void RenderItemPick();


	void RenderScene(FrameBuffer* frameBuffer, Program* shaderProgram);
	void RenderActiveObject(FrameBuffer* frameBuffer, Program* shaderProgram);
	void RenderActiveObjectOutline(FrameBuffer* frameBuffer, Program* shaderProgram);
	void RenderActiveObjectWireframe(FrameBuffer* frameBuffer, Program* shaderProgram, WireframeMode mode);

	Program* mOutlineProgram;
	Program* mSceneProgram;
	Program* mItemPickProgram;
	Program* mWireframeProgram;

	FrameBuffer* mSceneFrameBuffer;
	FrameBuffer* mItemPickFrameBuffer;
	Camera* mCamera;
	Texture2D* mWoodTexture;
	GameObject* mActiveObject;
	std::unordered_set<GameObject*> mGameObjects;

	int mPointSize;
	int mLineSize;
};