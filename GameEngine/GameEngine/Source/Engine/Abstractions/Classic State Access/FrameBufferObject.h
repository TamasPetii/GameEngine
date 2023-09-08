#pragma once
#include "../IFrameBufferObject.h"

namespace OpenGL::Classic
{
	class FrameBufferObject : public IFrameBufferObject
	{
	public:
		FrameBufferObject();
		~FrameBufferObject();
		void Bind() const override;
		void UnBind() const override;
	protected:
		void ReGenerate() override;
		void GenerateBuffer() override;
		void DeleteBuffer() override;
		GLuint GenerateTexture(const FboTextureInfo& info) override;
		GLuint GenerateRenderBuffer(const FboRenderBufferInfo& info) override;
	};
}

