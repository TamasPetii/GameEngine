#include "AnimationSystem.h"

void AnimationSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void AnimationSystem::OnUpdate(std::shared_ptr<Registry> registry, float deltaTime)
{
	auto resourceManager = ResourceManager::Instance();
	auto animationPool = registry->GetComponentPool<AnimationComponent>();

	if (!animationPool)
		return;

	std::for_each(std::execution::par, animationPool->GetDenseEntitiesArray().begin(), animationPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (animationPool->GetComponent(entity).animation != nullptr)
			{
				auto animationComponent = animationPool->GetComponent(entity);
				auto animation = animationComponent.animation;
				animationComponent.time = animation->GetTicksPerSecond() * deltaTime;
				animationComponent.time = fmod(animationComponent.time, animation->GetDuration());

				std::queue<std::pair<std::shared_ptr<NodeData>, glm::mat4>> queue;
				queue.push(std::make_pair(animation->GetRoot(), glm::mat4(1)));

				while (!queue.empty())
				{
					auto& [node, parentTransform] = queue.front();
					queue.pop();

					//Check if bone is a real bone (There can be wrong bones in bone list)
					auto it = std::find_if(animation->RefBones().begin(), animation->RefBones().end(),
						[&](const Bone& bone) -> bool {
							return bone.GetName() == node->name;
						});

					if (it != animation->RefBones().end())
					{
						Bone& bone = *it;
						bone.Update(deltaTime);
						auto transform = parentTransform * bone.GetTransform();
						auto index = animation->RefBoneInfos()[node->name].index;
						auto offset = animation->RefBoneInfos()[node->name].offset;
						animation->RefBoneTransforms()[index] = transform * offset;

						for (int i = 0; i < node->children.size(); i++)
							queue.push(std::make_pair(node->children[i], transform));
					}
				}
			}
		}
	);
}

void AnimationSystem::OnEnd(std::shared_ptr<Registry> registry)
{
}
