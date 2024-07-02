#include "AudioSystem.h"

void AudioSystem::OnStart(std::shared_ptr<Registry> registry)
{
}

void AudioSystem::OnUpdate(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto transformPool = registry->GetComponentPool<TransformComponent>();
	auto audioPool = registry->GetComponentPool<AudioComponent>();

	if (!transformPool || ! audioPool)
		return;

	static bool init = true;
	static glm::vec4* adTransfromSsboHandler = nullptr;

	if (init)
	{
		init = false;
		auto adTransfromSsbo = resourceManager->GetSsbo("AudioBillboard");
		adTransfromSsboHandler = static_cast<glm::vec4*>(adTransfromSsbo->MapBuffer(GL_WRITE_ONLY));
	}

	std::for_each(std::execution::seq, audioPool->GetDenseEntitiesArray().begin(), audioPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (true || audioPool->IsFlagSet(entity, UPDATE_FLAG) || (transformPool->HasComponent(entity) && transformPool->IsFlagSet(entity, CHANGED_FLAG)))
			{
				auto& audioComponent = audioPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto index = audioPool->GetIndex(entity);

				if (!audioComponent.sound && audioComponent.soundSource)
				{
					audioComponent.sound = SoundManager::Instance()->PlaySound(audioComponent.soundSource, audioComponent.isLooped);
				}

				if (audioComponent.sound)
				{
					audioComponent.sound->setPosition(irrklang::vec3d(transformComponent.translate.x, transformComponent.translate.y, transformComponent.translate.z));
					audioComponent.sound->setVolume(audioComponent.volume);
					audioComponent.sound->setPlaybackSpeed(audioComponent.speed);
					audioComponent.sound->setMinDistance(audioComponent.minDist);
					audioComponent.sound->setMaxDistance(audioComponent.maxDist);
				}
				
				adTransfromSsboHandler[index] = glm::vec4(transformComponent.translate, entity);
				audioPool->ResFlag(entity, UPDATE_FLAG);
			}
		}
	);

	//tsDataSsbo->UnMapBuffer();
}

void AudioSystem::OnEnd(std::shared_ptr<Registry> registry)
{
}

nlohmann::json AudioSystem::Serialize(Registry* registry, Entity entity)
{
	auto& transformComponent = registry->GetComponent<AudioComponent>(entity);
	nlohmann::json data;

	return data;
}

void AudioSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& transformComponent = registry->GetComponent<AudioComponent>(entity);

	registry->SetFlag<AudioComponent>(entity, UPDATE_FLAG);
}