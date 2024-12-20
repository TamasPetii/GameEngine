#pragma once
#include "EngineApi.h"
#include "Render/OpenGL/TextureGL.h"
#include <GL/glew.h>
#include <any>
#include <string>
#include <memory>
#include <unordered_map>
#include <glm/glm.hpp>

constexpr auto defaultFboParamTextureFunction = [](GLuint textureID) -> void {
	glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
};

struct ENGINE_API RenderbufferFboSpecGL
{
	GLint  width;
	GLint  height;
	GLuint component;
	GLuint attachment;
};

class ENGINE_API FramebufferGL
{
public:
	FramebufferGL();
	FramebufferGL(GLint width, GLint height);
	~FramebufferGL();
	void Bind(GLenum mode = GL_FRAMEBUFFER);
	void UnBind(GLenum mode = GL_FRAMEBUFFER);
	void Clear();
	void Resize(GLint width, GLint height);

	void AttachTexture(const std::string& name, TextureSpecGL& spec);
	void DeactivateTexture();
	void ActivateTexture(const std::string& name);
	void ActivateTexture(GLenum attachment);
	void ActivateTextures(const std::vector<std::string>& names);
	void ActivateTextures(const std::vector<GLenum>& attachments);
	void BindTexture(const std::string& name);
	void CheckCompleteness();

	const GLuint GetTextureID(const std::string& name);
	const GLuint64 GetTextureHandler(const std::string& name);

	std::any ReadPixel(const std::string& name, GLint x, GLint y);
	auto& GetTextureSpec(const std::string& name) { return m_Textures[name]->GetTextureSpec(); }
	auto& GetSize() { return m_Size; }
	void ClearStencil() { glClear(GL_STENCIL_BUFFER_BIT); }
	bool useFboSizeAsTexSize = true;
private:
	void RecreateResources();
	void DeleteResources();
private:
	glm::ivec2 m_Size;
	GLuint m_FramebufferID;
	std::unordered_map<std::string, std::shared_ptr<TextureGL>> m_Textures;
};

