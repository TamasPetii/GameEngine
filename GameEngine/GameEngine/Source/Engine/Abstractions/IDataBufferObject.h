#pragma once
#include "IBufferObject.h"
#include <GLEW/glew.h>
#include <vector>

namespace OpenGL
{
	template<typename T>
	class IDataBufferObject : public IBufferObject
	{
	public:
		virtual void AttachData(const std::vector<T>& data, BufferDrawMode mode) = 0;
		virtual void AttachSubData(const std::vector<T>& data, int startIndex = 0) = 0;

		#pragma region Getter
		inline const auto& Get_BufferId() const { return m_DataBufferId; }
		#pragma endregion
	protected:
		GLuint m_DataBufferId;
	};
}


