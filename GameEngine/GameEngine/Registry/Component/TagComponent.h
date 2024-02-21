#pragma once
#include <string>
#include <glm/glm.hpp>
#include <Registry/Component/Component.h>

class TagComponent : public Component
{
public:
	std::string name;
	std::string tag;
};
