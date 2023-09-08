#pragma once
#include "../IVertexArrayObject.h"

namespace OpenGL::Classic
{
	class VertexArrayObject : public IVertexArrayObject
	{
	public:
		VertexArrayObject();
		~VertexArrayObject();
		void Bind() const override;
		void UnBind() const override;
		void LinkAttribute(GLuint bufferId, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const override;
		void LinkAttributeI(GLuint bufferId, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const override;
	protected:
		void GenerateVertexArray() override;
		void DeleteVertexArray() override;
	};
}