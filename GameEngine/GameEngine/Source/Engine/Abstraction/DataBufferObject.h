#pragma once
#include "BufferObject.h"
#include "Vertex.h"
#include <vector>

namespace OpenGL::Classic
{
	template<typename T>
	class DataBufferObject : public BufferObject
	{
	public:
		DataBufferObject();
		~DataBufferObject();
		void Bind() const override;
		void UnBind() const override;
		void AttachData(const std::vector<T>& data, int mode);
		void AttachSubData(const std::vector<T>& data, int startIndex = 0);
		/*Getter*/
		inline const auto& Get_BufferId() const { return m_DataBufferId; }
	protected:
		void GenerateBuffer() override;
		void DeleteBuffer() override;
	private:
		GLuint m_DataBufferId;
	};

	using VertexBufferObject = DataBufferObject<Vertex>;
}

#include "DataBufferObject.inl"


