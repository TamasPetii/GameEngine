#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>


#include <random>
#include <string>
#include <unordered_map>
#include <execution>
#include <filesystem>


#include <Render/OpenGL/ProgramGL.h>
#include <Render/OpenGL/Camera.h>
#include <Render/OpenGL/ShaderStorageBufferGL.h>
#include <Render/OpenGL/UniformBufferGL.h>
#include <Render/OpenGL/Vertex.h>
#include <Render/OpenGL/VertexArrayGL.h>
#include <Render/OpenGL/TextureGL.h>
#include <Registry/Registry.h>
#include <Registry/Component/TransformComponent.h>
#include <Render/OpenGL/FramebufferGL.h>

class Renderer
{
public:
	static Renderer* Instance();
	static void Destroy();
	~Renderer();
	void Render();
	void Update();

	void InitFbo();
	GLuint m_fboID;
	GLuint m_fboColorID;
	GLuint m_fboDepthID;
	std::shared_ptr<Camera> m_Camera;
	std::shared_ptr<UniformBufferGL> m_CameraUbo;
	std::shared_ptr<FramebufferGL> m_Framebuffer;
private:
	Renderer();
	static Renderer* m_Instance;
	void InitPrograms();
	void InitUniformBuffers();
	void InitShaderStorageBuffers();
	std::unordered_map<std::string, std::unique_ptr<ProgramGL>> m_Programs;
	std::unordered_map<std::string, std::unique_ptr<UniformBufferGL>> m_UniformBuffers;
	std::unordered_map<std::string, std::unique_ptr<ShaderStorageBufferGL>> m_ShaderStorageBuffers;

	void InitTransforms();
	std::shared_ptr<ShaderStorageBufferGL> m_TransformSsbo;
	Registry m_Registry;
	const int count = 1000;

	void InitCube();
	std::shared_ptr<IndexBufferGL>  m_cubeIbo = nullptr;
	std::shared_ptr<VertexArrayGL>  m_cubeVao = nullptr;
	std::shared_ptr<VertexBufferGL> m_cubeVbo = nullptr;

	void InitTexture();
	std::unique_ptr<TextureGL> m_texture;
};

inline Renderer* Renderer::m_Instance = nullptr;