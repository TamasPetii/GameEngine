#pragma once
#include "../Engine/Engine.h"
#include "Entity/Entity.h"
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

	bool FindActiveEntity(int id);
	inline Camera* GetCamera() const { return mCamera; }
	inline std::unordered_set<Entity*>& GetEntities() { return mEntities; }
	inline Entity* GetActiveEntity() { return mActiveEntity; }
	inline void NoActiveObject() { mActiveEntity = nullptr; }
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
	void RenderScene(IFrameBufferObject* frameBuffer, Program* shaderProgram);
	void RenderItemPick();
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
	Program* mTestProgram;

	IFrameBufferObject* mSceneFrameBuffer;
	IFrameBufferObject* mItemPickFrameBuffer;
	IFrameBufferObject* mShadowFrameBuffer;


	Entity* mActiveEntity;
	std::unordered_set<Entity*> mEntities;

	Camera* mCamera;

	int mPointSize;
	int mLineSize;
	bool mRenderWireframePoints;
	glm::vec3 mWireframePointsColor;
	bool mRenderWireframeLines;
	glm::vec3 mWireframeLinesColor;
	bool mRenderWireframeNormals;
	glm::vec3 mWireframeNormalsColor;
};