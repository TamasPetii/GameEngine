#pragma once
#include "OpenGL/OpenGL.h"
#include "Utility/Camera.h"
#include "../Scene/Scene.h"
#include <unordered_set>
#include <set>
#include <map>
#include <fstream>

enum class WireframeMode
{
	POINT,
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
	void RenderScene(OpenGL::IFrameBufferObject* frameBuffer, OpenGL::ProgramObject* shaderProgram);
	void UploadLightsToShader(OpenGL::ProgramObject* shaderProgram);
	void RenderActiveObjectNormals(OpenGL::IFrameBufferObject* frameBuffer, OpenGL::ProgramObject* shaderProgram);



	void RenderActiveObjectOutline(OpenGL::IFrameBufferObject* frameBuffer, OpenGL::ProgramObject* shaderProgram);
	void RenderShadowMap(OpenGL::IFrameBufferObject* frameBuffer, OpenGL::ProgramObject* shaderProgram);
	void RenderSkyBox(OpenGL::IFrameBufferObject* frameBuffer);

	void RenderActiveObjectWireframe(OpenGL::IFrameBufferObject* frameBuffer, OpenGL::ProgramObject* shaderProgram, WireframeMode mode);
	void RenderGrid();
	


	std::unordered_map<std::string, OpenGL::ProgramObject*> m_ProgramObjects;
	std::unordered_map<std::string, OpenGL::IFrameBufferObject*> m_FrameBuffersObjects;

	Camera* m_Camera;
	Scene* m_Scene;

	std::vector<glm::mat4> transforms;

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

	void RenderEntity(Entity* entity, OpenGL::ProgramObject* shaderProgram, std::function<void(Entity*, OpenGL::ProgramObject*)> uploadToShader);
	void uploadToMainShader(Entity* entity, OpenGL::ProgramObject* shaderProgram);
	void uploadToShadowShader(Entity* entity, OpenGL::ProgramObject* shaderProgram);
	void uploadToOutlineShader(Entity* entity, OpenGL::ProgramObject* shaderProgram);
	void uploadToWireframeShader(Entity* entity, OpenGL::ProgramObject* shaderProgram);
};