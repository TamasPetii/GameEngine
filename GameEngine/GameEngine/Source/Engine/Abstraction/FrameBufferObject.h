#pragma once
#include "BufferObject.h"
#include <unordered_map>
#include <iostream>
#include <functional>
#include <string>
#include <any>
#include <algorithm>

namespace OpenGL::Classic
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

	class FrameBufferObject : public BufferObject
	{
	public:
		FrameBufferObject();
		~FrameBufferObject();
		void Bind() const override;
		void UnBind() const override;
		void ClearBuffer();
		void ResizeBuffer(GLint width, GLint height);
		void AttachTexture(const std::string& name, const FboTextureInfo& info);
		void AttachRenderBuffer(const std::string& name, const FboRenderBufferInfo& info);
		std::any ReadPixelData(const std::string& name, GLint x, GLint y);
		/*Getter*/
		inline const auto& Get_TextureId(const std::string& name) const { return m_Textures.at(name).first; }
	protected:
		void GenerateBuffer() override;
		void DeleteBuffer() override;
		void ReGenerateBuffer();
		GLuint GenerateTexture(const FboTextureInfo& info);
		GLuint GenerateRenderBuffer(const FboRenderBufferInfo& info);
	private:
		GLuint m_FrameBufferId;
		GLint m_Width;
		GLint m_Height;
		std::unordered_map<std::string, std::pair<GLuint, FboTextureInfo>> m_Textures;
		std::unordered_map<std::string, std::pair<GLuint, FboRenderBufferInfo>> m_RenderBuffers;
	};
}


