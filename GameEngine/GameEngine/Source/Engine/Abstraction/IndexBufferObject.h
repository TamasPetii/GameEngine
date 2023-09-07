#pragma once
#include "BufferObject.h"
#include <vector>

namespace OpenGL::Classic
{
	class IndexBufferObject : public BufferObject
	{
	public:
		IndexBufferObject();
		~IndexBufferObject();
		void Bind() const override;
		void UnBind() const override;
		void AttachData(const std::vector<GLuint>& data, int mode);
		void AttachSubData(const std::vector<GLuint>& data, int startIndex = 0);
	protected:
		void GenerateBuffer() override;
		void DeleteBuffer() override;
	private:
		GLuint m_IndexBufferId;
	};

}


