#include "Entity.h"

std::unordered_map<int, Entity*> Entity::mEntities;

Entity* Entity::Get(int id)
{
	if (mEntities.find(id) != mEntities.end())
		return mEntities[id];
	return nullptr;
}

Entity::Entity()
{
	static int id = 0;
	mId = id++;
	mText = "Entity" + std::to_string(mId);
	mEntities[mId] = this;
}

Entity::~Entity()
{
	for (auto& component : mComponents)
	{
		delete component.second;
	}
}

void Entity::AddChild(Entity* entity)
{
	entity->GetParent() = this;
	mChildren.push_back(entity);
}

void Entity::RemoveChild(Entity* entity)
{
	auto it = std::find(mChildren.begin(), mChildren.end(), entity);

	if (it != mChildren.end())
	{
		entity->GetParent() = nullptr;
		mChildren.erase(it);
	}
}

void Entity::Remove()
{
	if (mParent != nullptr)
	{
		mParent->RemoveChild(this);
	}
}

glm::mat4 Entity::GetParentTransformMatrix()
{
	if (mParent == nullptr) return glm::mat4(1);

	return mParent->GetParentTransformMatrix() * mParent->GetComponent<TransformComponent>()->GetTransformMatrix();
}