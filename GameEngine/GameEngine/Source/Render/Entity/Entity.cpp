#include "Entity.h"

Entity::Entity()
{
	static int id = 0;
	mId = id++;
	mText = "Entity";
}

Entity::~Entity()
{
	for (auto componenet : mComponents)
	{
		delete componenet.second;
	}
}