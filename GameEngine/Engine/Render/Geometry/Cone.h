#pragma once
#include "EngineApi.h"
#include "Geometry.h"
#include <glm/gtc/constants.hpp>

class ENGINE_API Cone : public Geometry
{
public:
	Cone();
	Cone(unsigned int count);
	virtual std::string GetName() { return "Cone"; }
protected:
	virtual void GenerateVertices() override;
	virtual void GenerateIndices() override;
	virtual void GenerateSurfacePoints() override;
private:
	float m_Radius;
	unsigned int m_Count;
};