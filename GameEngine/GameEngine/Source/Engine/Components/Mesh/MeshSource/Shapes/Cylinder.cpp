#include "Cylinder.h"

Shape<Cylinder>::Shape() : MeshBase()
{
	mCount = 25;
	mHeight = 2;
	mRadiusTop = 1;
	mRadiusBottom = 1;
	LoadVertices();
	CreateNormals();
	LoadIndices();
	UploadBuffersToGpu();
}

void Shape<Cylinder>::LoadVertices()
{
	mVertices.clear();

	std::vector<glm::vec3> topPoints;
	std::vector<glm::vec3> bottomCount;
	glm::vec3 topMiddle = glm::vec3(0, mHeight / 2, 0);
	glm::vec3 bottomMiddle = glm::vec3(0, -mHeight / 2, 0);

	for (int p = 0; p <= mCount; p++)
	{
		float theta = p / float(mCount) * (2 * M_PI);
		topPoints.push_back(glm::vec3(mRadiusTop * sinf(theta), mHeight / 2, mRadiusTop * cosf(theta)));
		bottomCount.push_back(glm::vec3(mRadiusBottom * sinf(theta), mHeight / -2, mRadiusBottom * cosf(theta)));
	}

	//Circle Top | 1 + mCount + 1
	mVertices.push_back
	({
		{ topMiddle },
		{ glm::vec2(0.5, 0.5)}
		});
	for (int p = 0; p <= mCount; p++)
	{
		float u = p / float(mCount);
		float theta = u * (2 * M_PI);
		mVertices.push_back
		({
			{ topPoints[p] },
			{ glm::vec2((cosf(theta) + 1) * 0.5, (sinf(theta) + 1) * 0.5)}
			});
	}

	//Circle Bottom | 1 + mCount + 1
	mVertices.push_back
	({
		{ bottomMiddle },
		{ glm::vec2(0.5, 0.5)}
		});
	for (int p = 0; p <= mCount; p++)
	{
		float u = p / float(mCount);
		float theta = u * (2 * M_PI);
		mVertices.push_back
		({
			{ bottomCount[p] },
			{ glm::vec2((sinf(theta) + 1) * 0.5, (cosf(theta) + 1) * 0.5)}
			});
	}

	//Wall Top
	for (int p = 0; p <= mCount; p++)
	{
		float u = p / float(mCount);
		float theta = u * (2 * M_PI);
		mVertices.push_back
		({
			{ topPoints[p] },
			{ glm::vec2(1 - u, 1)}
			});
	}

	//Wall Bottom
	for (unsigned int p = 0; p <= mCount; p++)
	{
		float u = p / float(mCount);
		float theta = u * (2 * M_PI);
		mVertices.push_back
		({
			{ bottomCount[p] },
			{ glm::vec2(1 - u, 0)}
			});
	}
}

void Shape<Cylinder>::CreateNormals()
{
	//Circle Top | 1 + mCount + 1
	int start = 0;
	for (int p = 0; p < mCount; p++)
	{
		glm::vec3 normal = MeshBase::GenerateNormalVectors(mVertices[start], mVertices[start + 1 + p], mVertices[start + 1 + p + 1]);
		mVertices[start].mNormal = normal;
		mVertices[start + 1 + p].mNormal = normal;
		mVertices[start + 1 + p + 1].mNormal = normal;
	}

	start = 1 + mCount + 1;
	for (int p = 0; p < mCount; p++)
	{
		glm::vec3 normal = MeshBase::GenerateNormalVectors(mVertices[start], mVertices[start + 1 + p + 1], mVertices[start + 1 + p]);
		mVertices[start].mNormal = normal;
		mVertices[start + 1 + p + 1].mNormal = normal;
		mVertices[start + 1 + p].mNormal = normal;
	}

	start = 2 * (1 + mCount + 1);
	for (int p = 0; p < mCount; p++)
	{
		glm::vec3 normal = MeshBase::GenerateNormalVectors(mVertices[start + p], mVertices[start + p + mCount + 1], mVertices[start + p + mCount + 2]);
		mVertices[start + p].mNormal = normal;
		mVertices[start + p + mCount + 1].mNormal = normal;
		mVertices[start + p + mCount + 2].mNormal = normal;
		mVertices[start + p].mNormal = normal;
		mVertices[start + p + mCount + 2].mNormal = normal;
		mVertices[start + p + 1].mNormal = normal;
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

	start = 2 * (1 + mCount + 1);
	for (int p = 0; p < mCount; p++)
	{
		mIndices.push_back(start + p);
		mIndices.push_back(start + p + mCount + 1);
		mIndices.push_back(start + p + mCount + 2);
		mIndices.push_back(start + p);
		mIndices.push_back(start + p + mCount + 2);
		mIndices.push_back(start + p + 1);
	}
}