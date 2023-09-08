#pragma once
#include "IBufferObject.h"
#include <GLEW/glew.h>
#include <vector>

namespace OpenGL
{
	class IIndexBufferObject : public IBufferObject
	{
	public:
		virtual void AttachData(const std::vector<GLuint>& data, BufferDrawMode mode) = 0;
		virtual void AttachSubData(const std::vector<GLuint>& data, int startIndex = 0) = 0;

		#pragma region Getter
		inline const auto& Get_BufferId() const { return m_IndexBufferId; }
		#pragma endregion
	protected:
		GLuint m_IndexBufferId;
	};
}
