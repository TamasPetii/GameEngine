#include "Vertex.h"

std::ostream& operator<<(std::ostream& os, const Vertex& vertex)
{
	os << "________Vertex_______" << std::endl;
	os << "Position: { " << vertex.mPosition.x << ", " << vertex.mPosition.y << ", " << vertex.mPosition.z << " }" << std::endl;
	os << "Normal: { " << vertex.mNormal.x << ", " << vertex.mNormal.y << ", " << vertex.mNormal.z << " }" << std::endl;
	os << "Tex: { " << vertex.mTexture.x << ", " << vertex.mTexture.y << " }" << std::endl;
	return os;
}