#pragma once
#include <Render/OpenGL/BufferGL.h>

class ShaderStorageBufferGL : public BufferGL
{
public:
	virtual void BindBufferBase(unsigned int layout) const override;
};

