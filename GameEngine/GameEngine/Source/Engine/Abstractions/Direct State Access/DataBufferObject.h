#pragma once
#include "../IDataBufferObject.h"
#include "../Shader Pipeline/Vertex.h"

namespace OpenGL::Dsa
{
	template<typename T>
	class DataBufferObject : public IDataBufferObject<T>
	{
	public:
		DataBufferObject();
		~DataBufferObject();
		void AttachData(const std::vector<T>& data, BufferDrawMode mode) override;
		void AttachSubData(const std::vector<T>& data, int startIndex = 0) override;
	protected:
		void GenerateBuffer() override;
		void DeleteBuffer() override;
	};

	using VertexBufferObject = DataBufferObject<Vertex>;
}

#include "DataBufferObject.inl"
