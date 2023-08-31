#pragma once
#include "../JsonObject.h"
#include "../Engine/Engine.h"
#include "Entity/Entity.h"

class Scene : public JsonObject
{
public:
	json SaveToJson() const override;
	void LoadFromJson(const json& object) override;

	Scene();
	Scene(const json& object);
	~Scene();

	void OnStart();
	void OnUpdate();

	void AttachEntity(Entity* entity);
	void DetachEntity(Entity* entity);
	void DeleteEntity(Entity* entity);

	/*Getter*/
	inline const auto& Get_Name() const { return m_Name; }
	inline const auto& Get_EntityList() const { return m_EntityList; }
	inline const auto& Get_ActiveEntity() const { return m_ActiveEntity; }
	/*Reference*/
	inline auto& Ref_Name() { return m_Name; }
	inline auto& Ref_EntityList() { return m_EntityList; }
	inline auto& Ref_ActiveEntity() { return m_ActiveEntity; }
private:
	std::string m_Name;
	Entity* m_ActiveEntity;
	std::vector<Entity*> m_EntityList;
	std::vector<Entity*> m_ToDelete;
};

