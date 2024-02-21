#include "UniformBufferGL.h"

void UniformBufferGL::BindBufferBase(unsigned int layout) const
{
	glBindBufferBase(GL_UNIFORM_BUFFER, layout, m_BufferID);
}
