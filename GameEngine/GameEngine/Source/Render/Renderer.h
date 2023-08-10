#pragma once
#include "../Engine/Engine.h"
#include "GameObjects/Shapes/Cube.h"
#include "GameObjects/Shapes/Sphere.h"
#include "GameObjects/Shapes/Torus.h"
#include "GameObjects/Shapes/Plane.h"
#include "GameObjects/Shapes/Cylinder.h"
#include "GameObjects/Lights/DirectionLight.h"
#include "GameObjects/Lights/PointLight.h"
#include <glm/gtc/quaternion.hpp>
#include <unordered_set>
#include <map>

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
	inline IFrameBufferObject* GetItemPickFrameBuffer() const { return mItemPickFrameBuffer; }
	inline IFrameBufferObject* GetSceneFrameBuffer() const { return mSceneFrameBuffer; }
	inline IFrameBufferObject* GetShadowFrameBuffer() const { return mShadowFrameBuffer; }

	inline Camera* GetCamera() const { return mCamera; }
	inline std::unordered_set<GameObject*>& GetGameObjects() { return mGameObjects; }
	inline GameObject* GetActiveObject() { return mActiveObject; }
	bool FindActiveObject(int id);
	inline void NoActiveObject() { mActiveObject = nullptr; }
	inline bool& GetRenderWireframePointsRef()  { return mRenderWireframePoints; }
	inline bool& GetRenderWireframeLinesRef()   { return mRenderWireframeLines; }
	inline bool& GetRenderWireframeNormalsRef() { return mRenderWireframeNormals; }
	inline glm::vec3& GetWireframePointsColorRef() { return mWireframePointsColor; }
	inline glm::vec3& GetWireframeLinesColorRef() { return mWireframeLinesColor; }
	inline glm::vec3& GetWireframeNormalsColorRef() { return mWireframeNormalsColor; }
private:
	void CreateStartScene();
	void PreRender();
	void PostRender();
	void RenderItemPick();
	void RenderScene(IFrameBufferObject* frameBuffer, Program* shaderProgram);
	void RenderActiveObject(IFrameBufferObject* frameBuffer, Program* shaderProgram);
	void RenderActiveObjectOutline(IFrameBufferObject* frameBuffer, Program* shaderProgram);
	void RenderActiveObjectWireframe(IFrameBufferObject* frameBuffer, Program* shaderProgram, WireframeMode mode);
	void RenderActiveObjectNormals(IFrameBufferObject* frameBuffer, Program* shaderProgram);
	void RenderShadowMap(IFrameBufferObject* frameBuffer, Program* shaderProgram);
	void UploadLightsToShader(Program* shaderProgram);

	Program* mOutlineProgram;
	Program* mSceneProgram;
	Program* mItemPickProgram;
	Program* mWireframeProgram;
	Program* mNormalsProgram;
	Program* mShadowProgram;

	IFrameBufferObject* mSceneFrameBuffer;
	IFrameBufferObject* mItemPickFrameBuffer;
	IFrameBufferObject* mShadowFrameBuffer;

	Camera* mCamera;
	Texture2D* mWoodTexture;
	GameObject* mActiveObject;

	std::unordered_set<GameObject*> mGameObjects;

	int mPointSize;
	int mLineSize;

	bool mRenderWireframePoints;
	glm::vec3 mWireframePointsColor;
	bool mRenderWireframeLines;
	glm::vec3 mWireframeLinesColor;
	bool mRenderWireframeNormals;
	glm::vec3 mWireframeNormalsColor;
};