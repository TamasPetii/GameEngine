#pragma once
#include "../IBindable.h"
#include "../IIndexBufferObject.h"

namespace OpenGL::Classic
{
	class IndexBufferObject : public IIndexBufferObject, public IBindable
	{
	public:
		IndexBufferObject();
		~IndexBufferObject();
		void Bind() const override;
		void UnBind() const override;
		void AttachData(const std::vector<GLuint>& data, BufferDrawMode mode) override;
		void AttachSubData(const std::vector<GLuint>& data, int startIndex = 0) override;
	protected:
		void GenerateBuffer() override;
		void DeleteBuffer() override;
	};
}