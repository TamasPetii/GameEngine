#include "CollisionCallback.h"
#include "Registry/Registry.h"
#include "Registry/Component/ScriptComponent.h"

void CollisionCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
    for (PxU32 i = 0; i < nbPairs; i++) {
        const PxContactPair& contactPair = pairs[i];

        if (contactPair.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
            //Retrieve the custom GameObject pointers
            Entity entityA = *static_cast<Entity*>(pairHeader.actors[0]->userData);
            Entity entityB = *static_cast<Entity*>(pairHeader.actors[1]->userData);

            if (registry->HasComponent<ScriptComponent>(entityA))
            {
                for (auto& [name, script] : registry->GetComponent<ScriptComponent>(entityA).scripts)
                {
                    if (script)
                    {  
                        script->OnCollisionEnter(entityB);
                    }
                }
            }

            if (registry->HasComponent<ScriptComponent>(entityB))
            {
                for (auto& [name, script] : registry->GetComponent<ScriptComponent>(entityB).scripts)
                {
                    if (script)
                    {
                        script->OnCollisionEnter(entityA);
                    }
                }
            }
        }

        if (contactPair.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {
            //Retrieve the custom GameObject pointers
            Entity entityA = *static_cast<Entity*>(pairHeader.actors[0]->userData);
            Entity entityB = *static_cast<Entity*>(pairHeader.actors[1]->userData);

            if (registry->HasComponent<ScriptComponent>(entityA))
            {
                for (auto& [name, script] : registry->GetComponent<ScriptComponent>(entityA).scripts)
                {
                    if (script)
                    {
                        script->OnCollisionExit(entityB);
                    }
                }
            }

            if (registry->HasComponent<ScriptComponent>(entityB))
            {
                for (auto& [name, script] : registry->GetComponent<ScriptComponent>(entityB).scripts)
                {
                    if (script)
                    {
                        script->OnCollisionExit(entityA);
                    }
                }
            }
        }
    }
}


void CollisionCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
    if (pairs->status & PxPairFlag::eNOTIFY_TOUCH_FOUND) {

        Entity entityA = null;
        Entity entityB = null;

        if (pairs[0].triggerActor && pairs[0].otherActor)
        {
            entityA = *static_cast<Entity*>(pairs[0].triggerActor->userData);
            entityB = *static_cast<Entity*>(pairs[0].otherActor->userData);
        }
        else if (pairs[1].triggerActor && pairs[1].otherActor)
        {
            entityA = *static_cast<Entity*>(pairs[1].triggerActor->userData);
            entityB = *static_cast<Entity*>(pairs[1].otherActor->userData);
        }
        else if (pairs[0].triggerActor && pairs[1].triggerActor)
        {
            entityA = *static_cast<Entity*>(pairs[0].triggerActor->userData);
            entityB = *static_cast<Entity*>(pairs[1].triggerActor->userData);
        }
        else
            return;

        if (registry->HasComponent<ScriptComponent>(entityA))
        {
            for (auto& [name, script] : registry->GetComponent<ScriptComponent>(entityA).scripts)
            {
                if (script)
                {
                    script->OnTriggerEnter(entityB);
                }
            }
        }

        if (registry->HasComponent<ScriptComponent>(entityB))
        {
            for (auto& [name, script] : registry->GetComponent<ScriptComponent>(entityB).scripts)
            {
                if (script)
                {
                    script->OnTriggerEnter(entityA);
                }
            }
        }
    }

    if (pairs->status & PxPairFlag::eNOTIFY_TOUCH_LOST) {
        Entity entityA = null;
        Entity entityB = null;

        if (pairs[0].triggerActor && pairs[0].otherActor)
        {
            entityA = *static_cast<Entity*>(pairs[0].triggerActor->userData);
            entityB = *static_cast<Entity*>(pairs[0].otherActor->userData);
        }
        else if (pairs[1].triggerActor && pairs[1].otherActor)
        {
            entityA = *static_cast<Entity*>(pairs[1].triggerActor->userData);
            entityB = *static_cast<Entity*>(pairs[1].otherActor->userData);
        }
        else if (pairs[0].triggerActor && pairs[1].triggerActor)
        {
            entityA = *static_cast<Entity*>(pairs[0].triggerActor->userData);
            entityB = *static_cast<Entity*>(pairs[1].triggerActor->userData);
        }
        else
            return;

        if (registry->HasComponent<ScriptComponent>(entityA))
        {
            for (auto& [name, script] : registry->GetComponent<ScriptComponent>(entityA).scripts)
            {
                if (script)
                {
                    script->OnTriggerExit(entityB);
                }
            }
        }

        if (registry->HasComponent<ScriptComponent>(entityB))
        {
            for (auto& [name, script] : registry->GetComponent<ScriptComponent>(entityB).scripts)
            {
                if (script)
                {
                    script->OnTriggerExit(entityA);
                }
            }
        }
    }
}

PxFilterFlags CollisionCallback::FilterShader(
    PxFilterObjectAttributes attributes0, PxFilterData filterData0,
    PxFilterObjectAttributes attributes1, PxFilterData filterData1,
    PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
    // let triggers through
    if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
    {
        pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
        return PxFilterFlag::eDEFAULT;
    }
    // generate contacts for all that were not filtered above
    pairFlags = PxPairFlag::eCONTACT_DEFAULT;

    // trigger the contact callback for pairs (A,B) where 
    // the filtermask of A contains the ID of B and vice versa.
    if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
    {
        pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
        pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
    }

    return PxFilterFlag::eDEFAULT;
}