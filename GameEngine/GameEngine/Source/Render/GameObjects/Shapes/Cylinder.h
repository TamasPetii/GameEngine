#pragma once
#include "Shape.h"

class Cylinder : public Shape
{
public:
	Cylinder();
	void CreateVertices() override;
	void CreateIndices() override;

	inline float& GetRadiusTopRef() { return mRadiusTop; }
	inline float& GetRadiusBottomRef() { return mRadiusBottom; }
	inline int& GetPointsRef() { return mPoints; }
private:
	int mPoints;
	float mHeight;
	float mRadiusTop;
	float mRadiusBottom;
};

