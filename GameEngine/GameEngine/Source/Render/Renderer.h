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
	inline FrameBuffer* GetSceneFrameBuffer() const { return mSceneFrameBuffer; }
	inline Camera* GetCamera() const { return mCamera; }
	inline std::unordered_set<GameObject*>& GetGameObjects() { return mGameObjects; }
private:
	void PreRender();
	void PostRender();
	void RenderScene();
	Program* mSceneProgram;
	FrameBuffer* mSceneFrameBuffer;
	Camera* mCamera;
	Texture2D* mWoodTexture;

	std::unordered_set<GameObject*> mGameObjects;
};

