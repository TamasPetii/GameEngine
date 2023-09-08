#pragma once
#include "IBindable.h"
#include "IBufferObject.h"
#include <GLEW/glew.h>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <iostream>
#include <string>
#include <any>

namespace OpenGL
{
	struct FboTextureInfo
	{
		FboTextureInfo() {}
		FboTextureInfo(GLint internalFormat, GLuint format, GLuint type, GLuint attachment, bool enableWrite = true) : internalFormat(internalFormat), format(format), type(type), attachment(attachment), enableWrite(enableWrite) {}
		inline void AttachClearCallback(const std::function<void(GLuint)>& callback) { clearCallBack = callback; }
		inline void AttachReadCallback(const std::function<std::any(GLuint, GLuint)>& callback) { readCallBack = callback; }

		bool enableWrite = true;
		GLint internalFormat;
		GLuint format;
		GLuint type;
		GLuint attachment;
		std::function<void(GLuint)> clearCallBack;
		std::function<std::any(GLuint, GLuint)> readCallBack;
	};

	struct FboRenderBufferInfo
	{
		FboRenderBufferInfo() {}
		FboRenderBufferInfo(GLuint component, GLuint attachment) : component(component), attachment(attachment) {}

		GLuint component;
		GLuint attachment;
	};

	class IFrameBufferObject : public IBufferObject, public IBindable
	{
	public:
		void Clear();
		void Resize(GLint width, GLint height);
		void AttachTexture(const std::string& name, const FboTextureInfo& info);
		void AttachRenderBuffer(const std::string& name, const FboRenderBufferInfo& info);
		std::any ReadPixelData(const std::string& name, GLint x, GLint y);

		#pragma region Getter
		inline const auto& Get_BufferId() const { return m_FrameBufferId; }
		inline const auto& Get_Width() const { return m_Width; }
		inline const auto& Get_Height() const { return m_Height; }
		inline const auto& Get_TextureId(const std::string& name) const { return m_Textures.at(name).first; }
		#pragma endregion
	protected:
		virtual void ReGenerate() = 0;
		void ReGenerateTexture();
		void ReGenerateRenderBuffer();
		virtual GLuint GenerateTexture(const FboTextureInfo& info) = 0;
		virtual GLuint GenerateRenderBuffer(const FboRenderBufferInfo& info) = 0;
	protected:
		GLuint m_FrameBufferId;
		GLint m_Width;
		GLint m_Height;
		std::unordered_map<std::string, std::pair<GLuint, FboTextureInfo>> m_Textures;
		std::unordered_map<std::string, std::pair<GLuint, FboRenderBufferInfo>> m_RenderBuffers;
	};
}

namespace OpenGL
{
	inline void IFrameBufferObject::AttachTexture(const std::string& name, const FboTextureInfo& info)
	{
		GLuint textureId = GenerateTexture(info);
		m_Textures[name] = std::make_pair(textureId, info);
	}

	inline void IFrameBufferObject::AttachRenderBuffer(const std::string& name, const FboRenderBufferInfo& info)
	{
		GLuint renderBufferId = GenerateRenderBuffer(info);
		m_RenderBuffers[name] = std::make_pair(renderBufferId, info);
	}

	inline void IFrameBufferObject::ReGenerateTexture()
	{
		for (auto& [name, pair] : m_Textures)
		{
			GLuint textureId = GenerateTexture(pair.second);
			pair.first = textureId;
		}
	}

	inline void IFrameBufferObject::ReGenerateRenderBuffer()
	{
		for (auto& [name, pair] : m_RenderBuffers)
		{
			GLuint renderBufferId = GenerateRenderBuffer(pair.second);
			pair.first = renderBufferId;
		}
	}

	inline std::any IFrameBufferObject::ReadPixelData(const std::string& name, GLint x, GLint y)
	{
		if (m_Textures.at(name).second.readCallBack)
		{
			Bind();
			std::any pixelData = m_Textures.at(name).second.readCallBack(x, y);
			UnBind();

			return pixelData;
		}
		else
		{
			throw std::runtime_error("No readCallBack in fbo");
		}
	}

	inline void IFrameBufferObject::Resize(GLint width, GLint height)
	{
		m_Width = width;
		m_Height = height;
		ReGenerate();
	}

	inline void IFrameBufferObject::Clear()
	{
		Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		for (auto& [name, pair] : m_Textures)
		{
			if (pair.second.clearCallBack)
				pair.second.clearCallBack(pair.first);
		}

		UnBind();
	}
}