#pragma once
#include "IBindable.h"
#include <GLEW/glew.h>

namespace OpenGL
{
	class IVertexArrayObject : public IBindable
	{
	public:
		virtual void LinkAttribute(GLuint bufferId, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const = 0;
		virtual void LinkAttributeI(GLuint bufferId, GLuint layout, GLint size, GLenum type, GLsizei stride, const GLvoid* offset) const = 0;

		#pragma region Getter
		inline const auto& Get_BufferId() const { return m_VertexArrayId; }
		#pragma endregion
	protected:
		virtual void GenerateVertexArray() = 0;
		virtual void DeleteVertexArray() = 0;
		GLuint m_VertexArrayId;
	};
}
