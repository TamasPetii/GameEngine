#pragma once
#include "Bindable.h"
#include <GLEW/glew.h>

namespace OpenGL::Classic
{
	class BufferObject : public Bindable
	{
	protected:
		virtual void GenerateBuffer() = 0;
		virtual void DeleteBuffer() = 0;
	};
}

