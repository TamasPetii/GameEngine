#include "Script.h"
#include "Registry/Registry.h"

Script::Script(std::shared_ptr<Registry> registry, Entity entity) :
	registry(registry),
	entity(entity)
{
}

Script::~Script()
{
}