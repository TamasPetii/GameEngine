#include "Scene.h"

Scene::Scene()
{
	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}