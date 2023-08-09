#include "Torus.h"

Torus::Torus() : Shape()
{
	mName = "Torus";
	mColumn = 50;
	mRow = 50;
	mRadiusPrimary = 0.75;
	mRadiusSecondary = 0.25;
	CreateVertices();
	CreateIndices();
	UploadBuffers();
}

void Torus::CreateVertices()
{
	mVertices.clear();
	for (unsigned int h = 0; h <= mRow; h++)
	{
		for (unsigned int w = 0; w <= mColumn; w++)
		{
			float u = w / (float)mColumn;
			float v = h / (float)mRow;
			float theta = u * (2 * M_PI);
			float fi = v * (2 * M_PI);

			mVertices.push_back
			({
				{ glm::vec3((mRadiusPrimary + mRadiusSecondary * cos(fi)) * cosf(theta) , mRadiusSecondary * sinf(fi), (mRadiusPrimary + mRadiusSecondary * cosf(fi)) * sinf(theta))},
				{ glm::vec3(cos(theta) * cosf(fi), sinf(fi), sinf(theta) * cosf(fi)) },
				{ glm::vec2(1 - u, 1 - v) }
			});
		}
	}
}

void Torus::CreateIndices()
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