#pragma once
#include "../IBindable.h"
#include "ShaderObject.h"
#include <GLM/glm.hpp>
#include <iostream>
#include <unordered_map>
#include "../../Utility/ImageTexture.h"

namespace OpenGL
{
	class ProgramObject : public IBindable
	{
	public:
		ProgramObject();
		ProgramObject(const std::vector<ShaderObject>& shaders, const std::vector<ShaderObjectInfo>& infos);
		~ProgramObject();
		void GenerateProgram();
		void DeleteProgram();
		void Bind() const override;
		void UnBind() const override;
		void AttachShader(const ShaderObject& shader);
		void AttachShaderInfo(const ShaderObjectInfo& info);

		template<typename T>
		inline void SetUniform(const std::string& name, const T& value);
		void SetUniformTexture(const std::string& name, const int sampler, GLuint id, GLuint type);
		void SetUniformTexture(const std::string& name, const int sampler, const ImageTexture* texture);
	protected:
		GLint GetUniformLocation(const std::string& name);
	private:
		GLuint m_ProgramId;
		std::vector<ShaderObject> m_Shaders;
		std::vector<ShaderObjectInfo> m_ShaderInfos;
		std::unordered_map<std::string, GLint> m_UniformLocations;
	};
}

#include "ProgramObject.inl"