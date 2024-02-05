#include "ShaderStorageBufferGL.h"

void ShaderStorageBufferGL::BindBufferBase(unsigned int layout) const
{
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, layout, m_BufferID);
}