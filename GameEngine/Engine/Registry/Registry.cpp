#include "Registry.h"
#include "Component/Components.h"
#include "System/Systems.h"

Registry::Registry()
{
}

Registry::Registry(nlohmann::json& data)
{
    DeSerialize(data);
}

Entity Registry::CreateEntity()
{
    Entity newEntity{ null };

    if (m_InactiveEntities.size() != 0)
    {
        newEntity = m_InactiveEntities.front();
        m_InactiveEntities.pop_front();
    }
    else
    {
        newEntity = m_NextEntity++;
        m_Parents.push_back(null);
        m_Children.push_back(std::vector<Child>{});
    }

    for (auto& [typeID, pool] : m_Pools)
        pool->AddNewEntity(newEntity);

    m_ActiveEntities.push_back(newEntity);
    return newEntity;
}

void Registry::DestroyEntity(Entity entity)
{
    for (auto& [typeID, pool] : m_Pools)
        pool->RemoveEntity(entity);

    auto it = std::find(m_ActiveEntities.begin(), m_ActiveEntities.end(), entity);

    m_ActiveEntities.erase(it);
    m_InactiveEntities.push_front(entity);
}

void Registry::SetParent(Entity entity, Parent parent)
{
    if (m_Parents[entity] != null)
    {
        auto& children = m_Children[m_Parents[entity]];
        auto it = std::find(children.begin(), children.end(), entity);
        children.erase(it);
        m_Parents[entity] = null;
    }

    m_Parents[entity] = parent;
    if (parent != null)
        m_Children[parent].push_back(entity);
}

bool Registry::IsDeepConnected(Entity entityA, Entity entityB)
{
    while (m_Parents[entityB] != null)
    {
        if (m_Parents[entityB] == entityA)
            return true;

        entityB = m_Parents[entityB];
    }

    return false;
}

nlohmann::json Registry::Serialize()
{
    nlohmann::json data;
    data["nextEntity"] = m_NextEntity;
    data["activeEntities"] = nlohmann::json::array();
    data["inactiveEntities"] = nlohmann::json::array();

    for (auto& entity : m_ActiveEntities)
        data["activeEntities"].push_back(SerializeEntity(entity));

    for (auto& entity : m_InactiveEntities)
        data["inactiveEntities"].push_back(entity);

    return data;
}

void Registry::DeSerialize(nlohmann::json& data)
{
    m_ActiveEntity = null;
    m_NextEntity = data["nextEntity"];
    m_Parents.resize(m_NextEntity, null);
    m_Children.resize(m_NextEntity);
    
	for (auto& entityData : data["inactiveEntities"])
        m_InactiveEntities.push_back(entityData);

    for (auto& entityData : data["activeEntities"])
        DeSerializeEntity(entityData);
}

void Registry::DeSerializeEntity(nlohmann::json& data)
{
	Entity entity = data["entity"];
	m_ActiveEntities.push_back(entity);
	m_Parents[entity] = data["parent"];

	for (auto& child : data["children"])
		m_Children[entity].push_back(child);

    auto& components = data["components"];

    if (components.contains("tagComponent"))
    {
        this->AddComponent<TagComponent>(entity);
        TagSystem::DeSerialize(this, entity, components["tagComponent"]);
    }

    if (components.contains("transformComponent"))
    {
        this->AddComponent<TransformComponent>(entity);
        TransformSystem::DeSerialize(this, entity, components["transformComponent"]);
    }

    if (components.contains("materialComponent"))
    {
        this->AddComponent<MaterialComponent>(entity);
        MaterialSystem::DeSerialize(this, entity, components["materialComponent"]);
    }

    if (components.contains("dirlightComponent"))
    {
        this->AddComponent<DirlightComponent>(entity);
        DirlightSystem::DeSerialize(this, entity, components["dirlightComponent"]);
    }

    if (components.contains("pointlightComponent"))
    {
        this->AddComponent<PointLightComponent>(entity);
        PointLightSystem::DeSerialize(this, entity, components["pointlightComponent"]);
    }

    if (components.contains("spotlightComponent"))
    {
        this->AddComponent<SpotLightComponent>(entity);
        SpotLightSystem::DeSerialize(this, entity, components["spotlightComponent"]);
    }

    if (components.contains("modelComponent"))
    {
        this->AddComponent<ModelComponent>(entity);
        ModelSystem::DeSerialize(this, entity, components["modelComponent"]);
    }

    if (components.contains("shapeComponent"))
    {
        this->AddComponent<ShapeComponent>(entity);
        ShapeSystem::DeSerialize(this, entity, components["shapeComponent"]);
    }

    /*
    if (components.contains("rigidbodyComponent"))
    {
        this->AddComponent<RigidbodyComponent>(entity);
        PhysicsSystem::DeSerialize(this, entity, components["rigidbodyComponent"]);
    }
    */

    if (components.contains("scriptComponent"))
    {
        this->AddComponent<ScriptComponent>(entity);
        ScriptSystem::DeSerialize(this, entity, components["scriptComponent"]);
    }

    if (components.contains("animationComponent"))
    {
        this->AddComponent<AnimationComponent>(entity);
        AnimationSystem::DeSerialize(this, entity, components["animationComponent"]);
    }

    if (components.contains("audioComponent"))
    {
        this->AddComponent<AudioComponent>(entity);
        AudioSystem::DeSerialize(this, entity, components["audioComponent"]);
    }

    if (components.contains("waterComponent"))
    {
        this->AddComponent<WaterComponent>(entity);
        WaterSystem::DeSerialize(this, entity, components["waterComponent"]);
    }

    /*
    if (components.contains("meshCollider"))
    {
        this->AddComponent<MeshCollider>(entity);
        MeshColliderSystem::DeSerialize(this, entity, components["meshCollider"]);
    }

    if (components.contains("sphereCollider"))
    {
        this->AddComponent<SphereCollider>(entity);
        SphereColliderSystem::DeSerialize(this, entity, components["sphereCollider"]);
    }
    */
}

nlohmann::json Registry::SerializeEntity(Entity entity)
{
    nlohmann::json data;
    data["entity"] = entity;
    data["parent"] = m_Parents[entity];
    data["children"] = nlohmann::json::array();
    data["components"] = nlohmann::json::object();

    for (auto& child : m_Children[entity])
        data["children"].push_back(child);
    
	if (HasComponent<TagComponent>(entity))
		data["components"]["tagComponent"] = TagSystem::Serialize(this, entity);

    if (HasComponent<TransformComponent>(entity))
        data["components"]["transformComponent"] = TransformSystem::Serialize(this, entity);

	if (HasComponent<MaterialComponent>(entity))
		data["components"]["materialComponent"] = MaterialSystem::Serialize(this, entity);

	if (HasComponent<DirlightComponent>(entity))
		data["components"]["dirlightComponent"] = DirlightSystem::Serialize(this, entity);

	if (HasComponent<PointLightComponent>(entity))
		data["components"]["pointlightComponent"] = PointLightSystem::Serialize(this, entity);

	if (HasComponent<SpotLightComponent>(entity))
		data["components"]["spotlightComponent"] = SpotLightSystem::Serialize(this, entity);

	if (HasComponent<ModelComponent>(entity))
		data["components"]["modelComponent"] = ModelSystem::Serialize(this, entity);

	if (HasComponent<ShapeComponent>(entity))
		data["components"]["shapeComponent"] = ShapeSystem::Serialize(this, entity);

    /*
	if (HasComponent<RigidbodyComponent>(entity))
		data["components"]["rigidbodyComponent"] = PhysicsSystem::Serialize(this, entity);
    */

	if (HasComponent<ScriptComponent>(entity))
		data["components"]["scriptComponent"] = ScriptSystem::Serialize(this, entity);

	if (HasComponent<AnimationComponent>(entity))
		data["components"]["animationComponent"] = AnimationSystem::Serialize(this, entity);

    if (HasComponent<AudioComponent>(entity))
        data["components"]["audioComponent"] = AudioSystem::Serialize(this, entity);

    if (HasComponent<WaterComponent>(entity))
        data["components"]["waterComponent"] = WaterSystem::Serialize(this, entity);
    /*
    if (HasComponent<MeshCollider>(entity))
        data["components"]["meshCollider"] = MeshColliderSystem::Serialize(this, entity);

    if (HasComponent<SphereCollider>(entity))
        data["components"]["sphereCollider"] = SphereColliderSystem::Serialize(this, entity);
    */

    return data;
}