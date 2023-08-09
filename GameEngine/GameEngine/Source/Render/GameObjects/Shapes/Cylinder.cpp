#include "Cylinder.h"

Cylinder::Cylinder() : Shape()
{
	mName = "Cylinder";
	mPoints = 25;
	mHeight = 2;
	mRadiusTop = 1;
	mRadiusBottom = 1;
	CreateVertices();
	CreateIndices();
	UploadBuffers();
}

void Cylinder::CreateVertices()
{
	mVertices.clear();

	std::vector<glm::vec3> topPoints;
	std::vector<glm::vec3> bottomPoints;
	glm::vec3 topMiddle = glm::vec3(0, mHeight / 2,0);
	glm::vec3 bottomMiddle = glm::vec3(0, -mHeight / 2, 0);

	for (int p = 0; p <= mPoints; p++)
	{
		float theta = p / float(mPoints) * (2 * M_PI);
		topPoints.push_back(glm::vec3(mRadiusTop * sinf(theta), mHeight / 2, mRadiusTop * cosf(theta)));
		bottomPoints.push_back(glm::vec3(mRadiusBottom * sinf(theta), mHeight / -2, mRadiusBottom * cosf(theta)));
	}

	//Circle Top | 1 + mPoints + 1
	mVertices.push_back
	({
		{ topMiddle },
		{ glm::vec3(0, 1, 0) },
		{ glm::vec2(0.5, 0.5)}
	});
	for (int p = 0; p <= mPoints; p++)
	{
		float u = p / float(mPoints);
		float theta = u * (2 * M_PI);
		mVertices.push_back
		({
			{ topPoints[p] },
			{ glm::vec3(0, 1, 0) },
			{ glm::vec2((cosf(theta) + 1) * 0.5, (sinf(theta) + 1) * 0.5)}
		});
	}

	//Circle Bottom | 1 + mPoints + 1
	mVertices.push_back
	({
		{ bottomMiddle },
		{ glm::vec3(0, -1, 0) },
		{ glm::vec2(0.5, 0.5)}
	});
	for (int p = 0; p <= mPoints; p++)
	{
		float u = p / float(mPoints);
		float theta = u * (2 * M_PI);
		mVertices.push_back
		({
			{ bottomPoints[p] },
			{ glm::vec3(0, -1, 0) },
			{ glm::vec2((sinf(theta) + 1) * 0.5, (cosf(theta) + 1) * 0.5)}
		});
	}

	//Wall Top
	for (int p = 0; p <= mPoints; p++)
	{
		float u = p / float(mPoints);
		float theta = u * (2 * M_PI);
		mVertices.push_back
		({
			{ topPoints[p] },
			{ glm::vec3(sinf(theta), mHeight / 2, cosf(theta)) },
			{ glm::vec2(1 - u, 1)}
		});
	}

	//Wall Bottom
	for (unsigned int p = 0; p <= mPoints; p++)
	{
		float u = p / float(mPoints);
		float theta = u * (2 * M_PI);
		mVertices.push_back
		({
			{ bottomPoints[p] },
			{ glm::vec3(sinf(theta), -mHeight / 2, cosf(theta)) },
			{ glm::vec2(1 - u, 0)}
		});
	}
}

void Cylinder::CreateIndices()
{
	mIndices.clear();

	//Circle Top | 1 + mPoints + 1
	int start = 0;
	for (int p = 0; p < mPoints; p++)
	{
		mIndices.push_back(start);
		mIndices.push_back(start + 1 + p);
		mIndices.push_back(start + 1 + p + 1);
	}

	start = 1 + mPoints + 1;
	for (int p = 0; p < mPoints; p++)
	{
		mIndices.push_back(start);
		mIndices.push_back(start + 1 + p + 1);
		mIndices.push_back(start + 1 + p);
	}

	start = 2 * (1 + mPoints + 1);
	for (int p = 0; p < mPoints; p++)
	{
		mIndices.push_back(start + p);
		mIndices.push_back(start + p + mPoints + 1);
		mIndices.push_back(start + p + mPoints + 2);
		mIndices.push_back(start + p);
		mIndices.push_back(start + p + mPoints + 2);
		mIndices.push_back(start + p + 1);
	}

}