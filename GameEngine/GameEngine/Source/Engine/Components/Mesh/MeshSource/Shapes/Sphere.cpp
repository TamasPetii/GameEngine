#include "Sphere.h"

Shape<Sphere>::Shape() : MeshBase()
{
    mColumn = 20;
    mRow = 20;
    mRadius = 1;

    LoadVertices();
    HardNormals();
    LoadIndices();
    UploadBuffersToGpu();
}

void Shape<Sphere>::LoadVertices()
{
    mPoints.clear();
    mVertices.clear();

    for (unsigned int h = 0; h <= mRow; h++)
    {
        for (unsigned int w = 0; w <= mColumn; w++)
        {
            float u = w / (float)mColumn;
            float v = h / (float)mRow;
            float theta = u * (2 * M_PI);
            float fi = M_PI * (0.5 - v);

            mPoints.push_back(Vertex(
                glm::vec3(mRadius * sinf(theta) * cosf(fi), mRadius * sinf(fi), mRadius * cosf(theta) * cosf(fi)),
                glm::vec3(sinf(theta) * cosf(fi), sinf(fi), cosf(theta) * cosf(fi)),
                glm::vec2(1 - u, 1 - v)
            ));
        }
    }

    for (unsigned int h = 0; h < mRow; h++)
    {
        for (unsigned int w = 0; w < mColumn; w++)
        {
            int c = w + h * (mColumn + 1);
            mVertices.push_back(mPoints[c]);
            mVertices.push_back(mPoints[c + mColumn + 1]);
            mVertices.push_back(mPoints[c + mColumn + 2]);
            mVertices.push_back(mPoints[c + 1]);
        }
    }
}

void Shape<Sphere>::ShadeNormals()
{
    int index = 0;
    for (unsigned int h = 0; h < mRow; h++)
    {
        for (unsigned int w = 0; w < mColumn; w++)
        {
            int c = w + h * (mColumn + 1);
            mVertices[index].mNormal = mPoints[c].mNormal;
            mVertices[index + 1].mNormal = mPoints[c + mColumn + 1].mNormal;
            mVertices[index + 2].mNormal = mPoints[c + mColumn + 2].mNormal;
            mVertices[index + 3].mNormal = mPoints[c + 1].mNormal;
            index += 4;
        }
    }
}

void Shape<Sphere>::HardNormals()
{  
    int index = 0;
    for (unsigned int h = 0; h < mRow; h++)
    {
        for (unsigned int w = 0; w < mColumn; w++)
        {
            int c = w + h * (mColumn + 1);
            glm::vec3 normal = MeshBase::GenerateNormalVectors(mVertices[index], mVertices[index+1], mVertices[index+2]);
            mVertices[index].mNormal = normal;
            mVertices[index + 1].mNormal = normal;
            mVertices[index + 2].mNormal = normal;
            mVertices[index + 3].mNormal = normal;
            index += 4;
        }
    }
}

void Shape<Sphere>::LoadIndices()
{
    mIndices.clear();

    for (unsigned int c = 0; c < mVertices.size(); c += 4)
    {
        mIndices.push_back(c);
        mIndices.push_back(c + 1);
        mIndices.push_back(c + 2);
        mIndices.push_back(c);
        mIndices.push_back(c + 2);
        mIndices.push_back(c + 3);
    }
}

void Shape<Sphere>::RefreshVertices()
{
    mVbo->AttachSubData(mVertices);
}