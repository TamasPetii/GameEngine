#include "Sphere.h"

Sphere::Sphere() : Shape()
{
	mName = "Sphere";
	mColumn = 50;
	mRow = 50;
	mRadius = 1;

	CreateVertices();
	CreateIndices();
	UploadBuffers();
}

void Sphere::CreateVertices()
{
	mVertices.clear();
	for (unsigned int h = 0; h <= mRow; h++)
	{
		for (unsigned int w = 0; w <= mColumn; w++)
		{
			float u = w / (float)mColumn;
			float v = h / (float)mRow;
			float theta = u * (2 * M_PI);
			float fi = M_PI * (0.5 - v);

			mVertices.push_back
			({
				{ glm::vec3(mRadius * sinf(theta) * cosf(fi), mRadius * sinf(fi), mRadius * cosf(theta) * cosf(fi)) },
				{ glm::vec3(sinf(theta) * cosf(fi), sinf(fi), cosf(theta) * cosf(fi)) },
				{ glm::vec2(1 - u, 1 - v) }
			});
		}
	}
}

void Sphere::CreateIndices()
{
	mIndices.clear();
	for (unsigned int h = 0; h < mRow; h++) //Last Line Removed
	{
		for (unsigned int w = 0; w < mColumn; w++) //Last Line Removed
		{
			int c = w + h * (mColumn + 1);
			mIndices.push_back(c);
			mIndices.push_back(c + mColumn + 1);
			mIndices.push_back(c + mColumn + 2);
			mIndices.push_back(c);
			mIndices.push_back(c + mColumn + 2);
			mIndices.push_back(c + 1);
		}
	}
}