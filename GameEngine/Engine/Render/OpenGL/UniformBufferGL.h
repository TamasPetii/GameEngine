#pragma once
#include "EngineApi.h"
#include "Render/OpenGL/BufferGL.h"

class ENGINE_API UniformBufferGL : public BufferGL
{
public:
	virtual void BindBufferBase(unsigned int layout) const override;
};

