#pragma once
#include "EngineApi.h"
#include <GL/glew.h>
#include <any>
#include <string>
#include <memory>
#include <iostream>
#include <functional>
#include <unordered_map>
#include <glm/glm.hpp>
#include <algorithm>

constexpr auto defaultFboParamTextureFunction = [](GLuint textureID) -> void {
	glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
};

struct ENGINE_API TextureFboSpecGL
{
	bool generateHandler = false;
	GLsizei layer{1};
	GLint   width{0};
	GLint   height{0};
	GLenum textureType;
	GLint  internalFormat;
	GLuint format;
	GLuint type;
	GLuint attachment;
	std::function<void(GLuint)> paramTextureFunction = defaultFboParamTextureFunction;
	std::function<void(GLuint, const TextureFboSpecGL&)> clearTextureFunction;
	std::function<std::any(GLuint, const TextureFboSpecGL&, GLint, GLint)> readTextureFunction;
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
	void AttachTexture(const std::string& name, const TextureFboSpecGL& spec);
	void AttachRenderbuffer(const std::string& name, const RenderbufferFboSpecGL& spec);
	void DeactivateTexture();
	void ActivateTexture(const std::string& name);
	void ActivateTexture(GLenum attachment);
	void ActivateTextures(const std::vector<std::string>& names);
	void ActivateTextures(const std::vector<GLenum>& attachments);
	void BindTexture(const std::string& name);
	void CheckCompleteness();
	std::any ReadPixel(const std::string& name, GLint x, GLint y);
	const GLuint GetTextureID(const std::string& name);
	auto& GetTextureSpec(const std::string& name) { return m_Textures[name].second; };
	auto& GetSize() { return m_Size; }
	void ClearStencil() { glClear(GL_STENCIL_BUFFER_BIT); }
private:
	void RecreateResources();
	void DeleteResources();
	glm::ivec2 m_Size;
	GLuint m_FramebufferID;
	std::unordered_map<std::string, std::pair<GLuint, TextureFboSpecGL>> m_Textures;
	std::unordered_map<std::string, std::pair<GLuint, RenderbufferFboSpecGL>> m_RenderBuffers;
};

