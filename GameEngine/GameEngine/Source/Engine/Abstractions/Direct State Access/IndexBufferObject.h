#pragma once
#include "../IIndexBufferObject.h"

namespace OpenGL::Dsa
{
	class IndexBufferObject : public IIndexBufferObject
	{
	public:
		IndexBufferObject();
		~IndexBufferObject();
		void AttachData(const std::vector<GLuint>& data, BufferDrawMode mode) override;
		void AttachSubData(const std::vector<GLuint>& data, int startIndex = 0) override;
	protected:
		void GenerateBuffer() override;
		void DeleteBuffer() override;
	};
}