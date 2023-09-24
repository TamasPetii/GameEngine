#include "Shape.h"

Shape::Shape() 
{
}

void Shape::GenerateShape()
{
    GenerateVertices();
    GenerateNormals();
    GenerateIndices();
    UploadToGpu();
}

void Shape::ReGenerateShape()
{
    GenerateVertices();
    GenerateNormals();
    GenerateIndices();
    UploadToGpu(true);
}