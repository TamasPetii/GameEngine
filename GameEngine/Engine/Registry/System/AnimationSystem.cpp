#include "AnimationSystem.h"
#include "Manager/ModelManager.h"

void AnimationSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void AnimationSystem::OnUpdate(std::shared_ptr<Registry> registry, float deltaTime)
{
	auto resourceManager = ResourceManager::Instance();
	auto animationPool = registry->GetComponentPool<AnimationComponent>();

	if (!animationPool)
		return;

	std::for_each(std::execution::seq, animationPool->GetDenseEntitiesArray().begin(), animationPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (animationPool->GetComponent(entity).animation != nullptr && animationPool->IsFlagSet(entity, REGENERATE_FLAG))
			{
				auto& animationComponent = animationPool->GetComponent(entity);
				animationComponent.boneTransforms.clear();
				animationComponent.boneTransforms.resize(animationComponent.animation->GetBoneCount());
				animationComponent.boneTransformSsbo = std::make_shared<ShaderStorageBufferGL>();
				animationComponent.boneTransformSsbo->BufferStorage(animationComponent.boneTransforms.size() * sizeof(glm::mat4), nullptr, GL_DYNAMIC_STORAGE_BIT);
				animationPool->ResFlag(entity, REGENERATE_FLAG);
			}
		}
	);

	std::for_each(std::execution::par, animationPool->GetDenseEntitiesArray().begin(), animationPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (animationPool->GetComponent(entity).animation != nullptr && (true || animationPool->IsFlagSet(entity, UPDATE_FLAG)))
			{
				auto& animationComponent = animationPool->GetComponent(entity);			
				CalculateBoneTransforms(animationComponent, deltaTime);
				animationPool->ResFlag(entity, UPDATE_FLAG);
				animationPool->SetFlag(entity, CHANGED_FLAG);
			}
		}
	);

	std::for_each(std::execution::seq, animationPool->GetDenseEntitiesArray().begin(), animationPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (animationPool->GetComponent(entity).animation != nullptr && (true || animationPool->IsFlagSet(entity, CHANGED_FLAG)))
			{
				auto& animationComponent = animationPool->GetComponent(entity);		
				animationComponent.boneTransformSsbo->BufferSubStorage(0, animationComponent.boneTransforms.size() * sizeof(glm::mat4), animationComponent.boneTransforms.data());
				animationPool->ResFlag(entity, CHANGED_FLAG);
			}
		}
	);
}

void AnimationSystem::CalculateBoneTransforms(AnimationComponent& animationComponent, float deltaTime)
{
	auto animation = animationComponent.animation;
	animationComponent.time += animation->GetTicksPerSecond() * deltaTime;
	animationComponent.time = fmod(animationComponent.time, animation->GetDuration());

	std::deque<std::pair<std::shared_ptr<NodeData>, glm::mat4>> queue;
	queue.push_front(std::make_pair(animation->GetRoot(), glm::mat4(1)));

	while (!queue.empty())
	{
		auto [node, parentTransform] = queue.front();
		queue.pop_front();

		std::string boneName = node->name;

		glm::mat4 boneTransform = node == animation->GetRoot() ? glm::mat4(1) : node->transform;

		//Check if bone is a real bone (There can be wrong bones in bone list)
		auto it = std::find_if(animation->RefBones().begin(), animation->RefBones().end(),
			[&](const Bone& bone) -> bool {
				return bone.GetName() == boneName;
			});

		if (it != animation->RefBones().end())
		{
			Bone& bone = *it;
			boneTransform = bone.Update(animationComponent.time);
		}

		glm::mat4 fullTransform = parentTransform * boneTransform;

		if (animation->RefBoneInfos().find(boneName) != animation->RefBoneInfos().end())
		{
			auto index = animation->RefBoneInfos()[boneName].index;
			auto offset = animation->RefBoneInfos()[boneName].offset;
			animationComponent.boneTransforms[index] = fullTransform * offset;
		}

		for (int i = node->children.size() - 1; i >= 0; --i)
		{
			auto child = node->children[i];
			queue.push_front(std::make_pair(child, fullTransform));
		}
	}
}

void AnimationSystem::OnEnd(std::shared_ptr<Registry> registry)
{
}

nlohmann::json AnimationSystem::Serialize(Registry* registry, Entity entity)
{
	auto& animationComponent = registry->GetComponent<AnimationComponent>(entity);

	nlohmann::json data;
	data["animation"] = animationComponent.animation ? animationComponent.animation->GetPath() : "none";

	return data;
}

void AnimationSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& animationComponent = registry->GetComponent<AnimationComponent>(entity);
	auto modelManager = ModelManager::Instance();

	animationComponent.animation = data["animation"] == "none" ? nullptr : modelManager->LoadAnimation(data["animation"]);

	registry->SetFlag<AnimationComponent>(entity, UPDATE_FLAG);
	registry->SetFlag<AnimationComponent>(entity, REGENERATE_FLAG);
}