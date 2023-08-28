#pragma once
#include <PHYSX/PxPhysicsAPI.h>
#include <PHYSX/geometry/PxBoxGeometry.h>

#include "../Engine/Engine.h"
#include "Entity/Entity.h"
#include <unordered_set>
#include <set>
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
	inline Entity*& GetShadowEntity() { return mShadowEntity; }
	inline Entity* GetActiveEntity() { return mActiveEntity; }
	inline void NoActiveObject() { mActiveEntity = nullptr; }
	inline bool& GetRenderWireframePointsRef()  { return mRenderWireframePoints; }
	inline bool& GetRenderWireframeLinesRef()   { return mRenderWireframeLines; }
	inline bool& GetRenderWireframeNormalsRef() { return mRenderWireframeNormals; }
	inline glm::vec3& GetWireframePointsColorRef() { return mWireframePointsColor; }
	inline glm::vec3& GetWireframeLinesColorRef() { return mWireframeLinesColor; }
	inline glm::vec3& GetWireframeNormalsColorRef() { return mWireframeNormalsColor; }

	void AddToDelete(Entity* entity) { mToDeleteEntities.push_back(entity); }
	void AddToErase(Entity* entity) { mToEraseEntities.push_back(entity); }

	physx::PxRigidDynamic* rigidCube;
	physx::PxRigidDynamic* rigidSphere;

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
	void RenderSkyBox(IFrameBufferObject* frameBuffer, Program* shaderProgram);
	void UploadLightsToShader(Program* shaderProgram);
	void RenderGrid();

	Program* mOutlineProgram;
	Program* mSceneProgram;
	Program* mItemPickProgram;
	Program* mWireframeProgram;
	Program* mNormalsProgram;
	Program* mShadowProgram;
	Program* mTestProgram;
	Program* mGridProgram;
	Program* mSkyboxProgram;
	Program* mSkysphereProgram;

	Shape<Plane>* mGrid;


	IFrameBufferObject* mSceneFrameBuffer;
	IFrameBufferObject* mItemPickFrameBuffer;
	IFrameBufferObject* mShadowFrameBuffer;

	Entity* mShadowEntity;
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

	std::vector<Entity*> mToDeleteEntities;
	std::vector<Entity*> mToEraseEntities;









	
	void InitPhysX();
	void CreateCubeRigidBody();
	const float GRAVITY = -9.81f;
	const float CUBE_DENSITY = 1.f;

	physx::PxRigidDynamic* rigidBody;

	physx::PxRigidDynamic* cubeRigidBody;
	physx::PxDefaultAllocator pxAllocator;
	physx::PxDefaultErrorCallback pxErrorCallback;
	physx::PxPhysics* pxPhysics = nullptr;
	physx::PxFoundation* pxFoundation = nullptr;
	physx::PxDefaultCpuDispatcher* pxDispatcher = nullptr;

	const std::string VISUAL_DEBUG_HOST = "127.0.0.1";
	const int VISUAL_DEBUG_PORT = 5425;
	const unsigned int VISUAL_DEBUG_TIMEOUT = 10;

	bool isDebugging = false;
	physx::PxPvd* pxVisualDebugger = nullptr;
	physx::PxPvdTransport* pxVisualDebugTransport = nullptr;

	physx::PxScene* pxScene = nullptr;
	physx::PxMaterial* pxMaterial = nullptr;

	physx::PxRigidStatic* groundRigidBody;
};