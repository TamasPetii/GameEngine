#pragma once
#include <GLEW/glew.h>

namespace OpenGL
{
	enum BufferDrawMode
	{
		STATIC = GL_STATIC_DRAW,
		DYNAMIC = GL_DYNAMIC_DRAW
	};

	class IBufferObject
	{
	protected:
		virtual void GenerateBuffer() = 0;
		virtual void DeleteBuffer() = 0;
	};
}