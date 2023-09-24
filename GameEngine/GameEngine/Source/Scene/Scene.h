#pragma once
#include "../Ecs/Manager/Managers.h"
#include "../Ecs/System/Systems.h"
#include "../ISerializable.h"

class Scene : public ISerializable
{
public:
	Scene();
	Scene(const json& data);
	~Scene();

	json Serialize() const override;
	void DeSerialize(const json& data) override;

	void OnStart();
	void OnUpdate(float deltaTime);

	bool IsActive(Ecs::Entity* entity);
	void AttachEntity(Ecs::Entity* entity);
	void DetachEntity(Ecs::Entity* entity);
	void AddToDelete(Ecs::Entity* entity);
	void AddToCopy(Ecs::Entity* entity);

	/*Getter*/
	inline const auto& Get_IsUpdating() const { return m_IsUpdating; }
	inline const auto& Get_EntityList() const { return m_EntityList; }
	inline const auto& Get_ActiveEntity() const { return m_ActiveEntity; }
	/*Reference*/
	inline auto& Ref_IsUpdating() { return m_IsUpdating; }
	inline auto& Ref_EntityList() { return m_EntityList; }
	inline auto& Ref_ActiveEntity() { return m_ActiveEntity; }
private:
	void EmptyToDelete();
	void EmtpyToCopy();
private:
	bool m_IsUpdating = false;
	Ecs::Entity* m_ActiveEntity;
	std::vector<Ecs::Entity*> m_EntityList;
	std::vector<Ecs::Entity*> m_ToDelete;
	std::vector<Ecs::Entity*> m_ToCopy;
};

