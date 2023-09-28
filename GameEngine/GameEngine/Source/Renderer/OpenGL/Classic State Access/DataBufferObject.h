#pragma once
#include <GLM/glm.hpp>
#include "../IBindable.h"
#include "../IDataBufferObject.h"
#include "../Shader Pipeline/Vertex.h"

namespace OpenGL::Classic
{
	template<typename T>
	class DataBufferObject : public IDataBufferObject<T>, public IBindable
	{
	public:
		DataBufferObject();
		~DataBufferObject();
		void Bind() const override;
		void UnBind() const override;
		void AttachData(const std::vector<T>& data, BufferDrawMode mode) override;
		void AttachSubData(const std::vector<T>& data, int startIndex = 0) override;
	protected:
		void GenerateBuffer() override;
		void DeleteBuffer() override;
	};

	using VertexBufferObject = DataBufferObject<Vertex>;
	using MatrixBufferObject = DataBufferObject<glm::mat4>;
}

#include "DataBufferObject.inl"
