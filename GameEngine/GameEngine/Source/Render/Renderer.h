#pragma once
#include <PHYSX/PxPhysicsAPI.h>
#include <PHYSX/geometry/PxBoxGeometry.h>

#include "../Engine/Engine.h"
#include <unordered_set>
#include <set>
#include <map>
#include <fstream>

#include "../Scene/Scene.h"

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

	inline bool& GetRenderWireframePointsRef()  { return mRenderWireframePoints; }
	inline bool& GetRenderWireframeLinesRef()   { return mRenderWireframeLines; }
	inline bool& GetRenderWireframeNormalsRef() { return mRenderWireframeNormals; }
	inline glm::vec3& GetWireframePointsColorRef() { return mWireframePointsColor; }
	inline glm::vec3& GetWireframeLinesColorRef() { return mWireframeLinesColor; }
	inline glm::vec3& GetWireframeNormalsColorRef() { return mWireframeNormalsColor; }

	#pragma region Getter
	inline const auto& Get_Camera() const { return m_Camera; }
	inline const auto& Get_Scene() const { return m_Scene; }
	inline const auto& Get_ProgramObject(const std::string& program) const { return m_ProgramObjects.at(program); }
	inline const auto& Get_FrameBufferObject(const std::string& frameBuffer) const { return m_FrameBuffersObjects.at(frameBuffer); }
	#pragma endregion

	#pragma region Reference
	inline auto& Ref_Camera() { return m_Camera; }
	inline auto& Ref_Scene() { return m_Scene; }
	inline auto& Get_ProgramObject(const std::string& program) { return m_ProgramObjects.at(program); }
	inline auto& Get_FrameBufferObject(const std::string& frameBuffer) { return m_FrameBuffersObjects.at(frameBuffer); }
	#pragma endregion

private:
	void CreateStartScene();
	void PreRender();
	void PostRender();
	void RenderScene(OpenGL::Classic::FrameBufferObject* frameBuffer, OpenGL::Classic::ProgramObject* shaderProgram);
	void UploadLightsToShader(OpenGL::Classic::ProgramObject* shaderProgram);
	void RenderActiveObjectNormals(OpenGL::Classic::FrameBufferObject* frameBuffer, OpenGL::Classic::ProgramObject* shaderProgram);



	void RenderActiveObjectOutline(OpenGL::Classic::FrameBufferObject* frameBuffer, OpenGL::Classic::ProgramObject* shaderProgram);
	void RenderShadowMap(OpenGL::Classic::FrameBufferObject* frameBuffer, OpenGL::Classic::ProgramObject* shaderProgram);
	void RenderSkyBox(OpenGL::Classic::FrameBufferObject* frameBuffer);

	void RenderActiveObjectWireframe(OpenGL::Classic::FrameBufferObject* frameBuffer, OpenGL::Classic::ProgramObject* shaderProgram, WireframeMode mode);
	void RenderGrid();
	


	std::unordered_map<std::string, OpenGL::Classic::ProgramObject*> m_ProgramObjects;
	std::unordered_map<std::string, OpenGL::Classic::FrameBufferObject*> m_FrameBuffersObjects;

	Camera* m_Camera;
	Scene* m_Scene;



	int mPointSize;
	int mLineSize;
	bool mRenderWireframePoints;
	glm::vec3 mWireframePointsColor;
	bool mRenderWireframeLines;
	glm::vec3 mWireframeLinesColor;
	bool mRenderWireframeNormals;
	glm::vec3 mWireframeNormalsColor;

	public:
		float heightScale = 0.1;

	void RenderEntity(Entity* entity, OpenGL::Classic::ProgramObject* shaderProgram, std::function<void(Entity*, OpenGL::Classic::ProgramObject*)> uploadToShader);
	void uploadToMainShader(Entity* entity, OpenGL::Classic::ProgramObject* shaderProgram);
	void uploadToShadowShader(Entity* entity, OpenGL::Classic::ProgramObject* shaderProgram);
	void uploadToOutlineShader(Entity* entity, OpenGL::Classic::ProgramObject* shaderProgram);
	void uploadToWireframeShader(Entity* entity, OpenGL::Classic::ProgramObject* shaderProgram);
};