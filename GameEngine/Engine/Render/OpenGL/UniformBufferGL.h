#pragma once
#include <Render/OpenGL/BufferGL.h>

class UniformBufferGL : public BufferGL
{
public:
	virtual void BindBufferBase(unsigned int layout) const override;
};

