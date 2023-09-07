#pragma once
#include <GLEW/glew.h>
#include <fstream>
#include <string>
#include <vector>

namespace OpenGL
{
	class ShaderObject
	{
	public:
		ShaderObject(GLuint type, const std::string& path);
		~ShaderObject();

		/*Getter*/
		inline const auto& Get_ShaderId() const { return m_ShaderId; }
	private:
		GLuint m_ShaderId;
	};

	class ShaderObjectInfo
	{
	public:
		ShaderObjectInfo(GLuint location, const std::string& name) : m_Location(location), m_Name(name) {}

		/*Getter*/
		inline const auto& Get_Location() const { return m_Location; }
		inline const auto& Get_Name() const { return m_Name; }
	private:
		GLuint m_Location;
		std::string m_Name;
	};
}


