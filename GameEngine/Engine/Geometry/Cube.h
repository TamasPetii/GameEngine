#pragma once
#include "Geometry.h"

class Cube : public Geometry
{
public:
	Cube();
protected:
	virtual void GenerateVertices() override;
	virtual void GenerateIndices() override;
	virtual void GenerateSurfacePoints() override;
private:
	enum CubeSurfacePositions
	{
		FRONT_TOP_LEFT,
		FRONT_BOTTOM_LEFT,
		FRONT_BOTTOM_RIGHT,
		FRONT_TOP_RIGHT,
		BACK_TOP_LEFT,
		BACK_BOTTOM_LEFT,
		BACK_BOTTOM_RIGHT,
		BACK_TOP_RIGHT
	};
};

