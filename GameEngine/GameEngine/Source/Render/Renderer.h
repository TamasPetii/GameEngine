#pragma once
#include "../Engine/Engine.h"
#include "GameObjects/Shapes/Cube.h"
#include <unordered_set>

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
private:
	void PreRender();
	void PostRender();
	void RenderScene();
	void RenderItemPick();
	Program* mOutlineProgram;
	Program* mSceneProgram;
	FrameBuffer* mSceneFrameBuffer;
	Program* mItemPickProgram;
	FrameBuffer* mItemPickFrameBuffer;
	Camera* mCamera;
	Texture2D* mWoodTexture;
	GameObject* mActiveObject;

	std::unordered_set<GameObject*> mGameObjects;
};

