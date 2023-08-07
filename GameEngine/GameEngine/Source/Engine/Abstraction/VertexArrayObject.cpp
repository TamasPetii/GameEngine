#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &mVertexArrayId);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &mVertexArrayId);
}

void VertexArrayObject::Bind() const
{
	glBindVertexArray(mVertexArrayId);
}

void VertexArrayObject::UnBind() const
{
	glBindVertexArray(0);
}