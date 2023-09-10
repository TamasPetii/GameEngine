#pragma once
#include "../JsonObject.h"
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
	void OnUpdate(float deltaTime);

	void AttachEntity(Entity* entity);
	void DetachEntity(Entity* entity);
	void AddToDelete(Entity* entity);
	void AddToCopy(Entity* entity);
	bool IsActive(Entity* entity);


	/*Getter*/
	inline const auto& Get_IsUpdating() const { return m_IsUpdating; }
	inline const auto& Get_Name() const { return m_Name; }
	inline const auto& Get_EntityList() const { return m_EntityList; }
	inline const auto& Get_ActiveEntity() const { return m_ActiveEntity; }
	/*Reference*/
	inline auto& Ref_IsUpdating() { return m_IsUpdating; }
	inline auto& Ref_Name() { return m_Name; }
	inline auto& Ref_EntityList() { return m_EntityList; }
	inline auto& Ref_ActiveEntity() { return m_ActiveEntity; }
private:
	bool m_IsUpdating = false;
	std::string m_Name;
	Entity* m_ActiveEntity;
	std::vector<Entity*> m_EntityList;
	std::vector<Entity*> m_ToDelete;
	std::vector<Entity*> m_ToCopy;
};

