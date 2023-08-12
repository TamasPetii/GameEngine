#include "Torus.h"

Shape<Torus>::Shape() : MeshBase()
{
    mColumn = 50;
    mRow = 50;
    mRadiusPrimary = 0.75;
    mRadiusSecondary = 0.25;

    LoadVertices();
    HardNormals();
    LoadIndices();
    UploadBuffersToGpu();
}

void Shape<Torus>::LoadVertices()
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
            float fi = v * (2 * M_PI);

            mPoints.push_back
            ({
                glm::vec3((mRadiusPrimary + mRadiusSecondary * cos(fi)) * cosf(theta) , mRadiusSecondary * sinf(fi), (mRadiusPrimary + mRadiusSecondary * cosf(fi)) * sinf(theta)),
                glm::vec3(cos(theta) * cosf(fi), sinf(fi), sinf(theta) * cosf(fi)),
                glm::vec2(1 - u, 1 - v)
            });
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

void Shape<Torus>::ShadeNormals()
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

void Shape<Torus>::HardNormals()
{
    int index = 0;
    for (unsigned int h = 0; h < mRow; h++)
    {
        for (unsigned int w = 0; w < mColumn; w++)
        {
            int c = w + h * (mColumn + 1);
            glm::vec3 normal = MeshBase::GenerateNormalVectors(mPoints[c], mPoints[c + mColumn + 1], mPoints[c + mColumn + 2]);
            mVertices[index].mNormal = normal;
            mVertices[index + 1].mNormal = normal;
            mVertices[index + 2].mNormal = normal;
            mVertices[index + 3].mNormal = normal;
            index += 4;
        }
    }
}

void Shape<Torus>::LoadIndices()
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

void Shape<Torus>::RefreshVertices()
{
    mVbo->AttachSubData(mVertices);
}