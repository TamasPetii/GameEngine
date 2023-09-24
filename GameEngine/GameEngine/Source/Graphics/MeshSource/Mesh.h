#pragma once
#include <GLM/glm.hpp>

#include "../Abstraction/OpenGL.h"
#include "../../IRenderable.h"
#include "../../ISerializable.h"
#include "../../IClonable.h"

class Mesh : public IRenderable, public ISerializable, public ICloneable
{
public:
	Mesh();
	~Mesh();
	static void GenerateTbn(Vertex& vertex1, Vertex& vertex2, Vertex& vertex3, bool isAveraged = false);
	static void GenerateTbn(Vertex& vertex, bool isAveraged = false);
protected:
	void UploadToGpu(bool IsReuploaded = false);
protected:
	GLuint m_IndexCount;
	GLuint m_VertexCount;
	std::vector<GLuint> m_Indices;
	std::vector<Vertex> m_Vertices;
	OpenGL::Classic::VertexArrayObject* m_Vao;
	OpenGL::Classic::VertexBufferObject* m_Vbo;
	OpenGL::Classic::IndexBufferObject* m_Ibo;
};

