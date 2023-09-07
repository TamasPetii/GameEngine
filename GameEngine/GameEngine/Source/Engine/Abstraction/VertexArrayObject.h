#pragma once
#include "BufferObject.h"
#include "DataBufferObject.h"

namespace OpenGL::Classic
{
	class VertexArrayObject : public Bindable
	{
	public:
		VertexArrayObject();
		~VertexArrayObject();
		void Bind() const override;
		void UnBind() const override;
		template<typename T>
		void LinkAttribute(const DataBufferObject<T>* buffer, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const;
		template<typename T>
		void LinkAttributeI(const DataBufferObject<T>* buffer, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const;
	protected:
		void GenerateVertexArray();
		void DeleteVertexArray();
	private:
		GLuint m_VertexArrayId;
	};
}

#include "VertexArrayObject.inl"

