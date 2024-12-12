#pragma once
#include "EngineApi.h"

class ENGINE_API BenchmarkManager
{
public:
	static void AddToRenderedTriangleCount(unsigned int triangleCount) { renderedTriangleCount += triangleCount; }
	static void ClearRenderedTriangleCount() { renderedTriangleCount = 0; }
	static void AddToRenderedEntityCount(unsigned int entityCount) { renderedEntityCount += entityCount; }
	static void ClearRenderedEntityCount() { renderedEntityCount = 0; }
	static void AddToRenderedInstanceCount(unsigned int instanceCount) { renderedInstanceCount += instanceCount; }
	static void ClearRenderedInstanceCount() { renderedInstanceCount = 0; }

	static unsigned int GetRenderedEntityCount() { return renderedEntityCount; }
	static unsigned int GetRenderedInstanceCount() { return renderedInstanceCount; }
	static unsigned int GetRenderedTriangleCount() { return renderedTriangleCount; }
	static void ResetRenderedEntityCount() { renderedEntityCount = 0; }
	static void ResetRenderedInstanceCount() { renderedInstanceCount = 0; }
	static void ResetRenderedTriangleCount() { renderedTriangleCount = 0; }
private:
	static unsigned int renderedEntityCount;
	static unsigned int renderedInstanceCount;
	static unsigned int renderedTriangleCount;
};

