#include "Cylinder.h"

Shape<Cylinder>::Shape() : MeshBase()
{
	mCount = 25;
	mHeight = 2;
	mRadiusTop = 1;
	mRadiusBottom = 1;
	LoadVertices();
	HardNormals();
	LoadIndices();
	UploadBuffersToGpu();
}

void Shape<Cylinder>::LoadVertices()
{
	mPoints.clear();
	mVertices.clear();

	mPoints.push_back(Vertex(glm::vec3(0, mHeight / 2, 0), glm::vec3(0, 1, 0), glm::vec2(0.5, 0.5)));
	mPoints.push_back(Vertex(glm::vec3(0, -mHeight / 2, 0), glm::vec3(0, -1, 0), glm::vec2(0.5, 0.5)));
	for (int p = 0; p <= mCount; p++)
	{
		float u = p / float(mCount);
		float theta = u * (2 * M_PI);
		mPoints.push_back(Vertex(glm::vec3(mRadiusTop * sinf(theta), mHeight / 2, mRadiusTop * cosf(theta)), glm::vec3(sinf(theta), 0, cosf(theta)),glm::vec2(1 - u, 1)));
		mPoints.push_back(Vertex(glm::vec3(mRadiusBottom * sinf(theta), mHeight / -2, mRadiusBottom * cosf(theta)), glm::vec3(sinf(theta), 0, cosf(theta)), glm::vec2(1 - u, 0)));
	}

	//Top Vertices | 1 + mCount + 1
	mVertices.push_back(mPoints[0]);
	for (int p = 0; p <= mCount; p++)
	{
		int c = 2 + p * 2;
		float theta = p / float(mCount) * (2 * M_PI);
		mVertices.push_back(Vertex(
			mPoints[c].mPosition,
			glm::vec3(0, 1, 0),
			glm::vec2((cosf(theta) + 1) * 0.5, (sinf(theta) + 1) * 0.5)			
		));
	}

	//Top Vertices | 1 + mCount + 1
	mVertices.push_back(mPoints[1]);
	for (int p = 0; p <= mCount; p++)
	{
		int c = 2 + 1 + p * 2;
		float theta = p / float(mCount) * (2 * M_PI);
		mVertices.push_back(Vertex(
			mPoints[c].mPosition,
			glm::vec3(0, -1, 0),
			glm::vec2((sinf(theta) + 1) * 0.5, (cosf(theta) + 1) * 0.5)
		));
	}

	for (int p = 0; p < mCount; p++)
	{
		int c = 2 + p * 2;
		mVertices.push_back(Vertex(mPoints[c].mPosition, mPoints[c].mTexture));
		mVertices.push_back(Vertex(mPoints[c + 1].mPosition, mPoints[c + 1].mTexture));
		mVertices.push_back(Vertex(mPoints[c + 3].mPosition, mPoints[c + 3].mTexture));
		mVertices.push_back(Vertex(mPoints[c + 2].mPosition, mPoints[c + 2].mTexture));
	}
}

void Shape<Cylinder>::HardNormals()
{
	int index = 0;
	int start = 2 * (1 + mCount + 1);
	for (int p = 0; p < mCount; p++)
	{
		int c = start + index;
		glm::vec3 normal = MeshBase::GenerateNormalVectors(mVertices[c], mVertices[c + 1], mVertices[c + 2]);
		mVertices[c].mNormal = normal;
		mVertices[c+1].mNormal = normal;
		mVertices[c+2].mNormal = normal;
		mVertices[c+3].mNormal = normal;
		index += 4;
	}
}

void Shape<Cylinder>::ShadeNormals()
{
	int index = 0;
	int start = 2 * (1 + mCount + 1);
	for (int p = 0; p < mCount; p++)
	{
		int x = 2 + p * 2;
		int c = start + index;
		mVertices[c].mNormal = mPoints[x].mNormal;
		mVertices[c + 1].mNormal = mPoints[x+1].mNormal;
		mVertices[c + 2].mNormal = mPoints[x+2].mNormal;
		mVertices[c + 3].mNormal = mPoints[x+3].mNormal;
		index += 4;
	}
}


void Shape<Cylinder>::LoadIndices()
{
	mIndices.clear();

	//Circle Top | 1 + mCount + 1
	int start = 0;
	for (int p = 0; p < mCount; p++)
	{
		mIndices.push_back(start);
		mIndices.push_back(start + 1 + p);
		mIndices.push_back(start + 1 + p + 1);
	}

	start = 1 + mCount + 1;
	for (int p = 0; p < mCount; p++)
	{
		mIndices.push_back(start);
		mIndices.push_back(start + 1 + p + 1);
		mIndices.push_back(start + 1 + p);
	}

	int index = 0;
	start = 2 * (1 + mCount + 1);
	for (int p = 0; p < mCount; p++)
	{
		int c = start + index;
		mIndices.push_back(c);
		mIndices.push_back(c+1);
		mIndices.push_back(c+2);

		mIndices.push_back(c);
		mIndices.push_back(c+2);
		mIndices.push_back(c+3);

		index += 4;
	}
}

void Shape<Cylinder>::RefreshVertices()
{
	mVbo->AttachSubData(mVertices);
}