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

	auto adTransfromSsbo = resourceManager->GetSsbo("AudioBillboard");
	if (adTransfromSsbo->GetBufferHandler() == nullptr)
		adTransfromSsbo->MapBufferRange();
	glm::vec4* adTransfromSsboHandler = static_cast<glm::vec4*>(adTransfromSsbo->GetBufferHandler());

	if (!adTransfromSsboHandler || audioPool->GetSize() > resourceManager->GetComponentSsboSize<AudioComponent>())
		return;

	std::for_each(std::execution::seq, audioPool->GetDenseEntitiesArray().begin(), audioPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			if (true || audioPool->IsFlagSet(entity, UPDATE_FLAG) || (transformPool->HasComponent(entity) && transformPool->IsFlagSet(entity, CHANGED_FLAG)))
			{
				auto& audioComponent = audioPool->GetComponent(entity);
				auto& transformComponent = transformPool->GetComponent(entity);
				auto index = audioPool->GetIndex(entity);

				if (!audioComponent.sound && audioComponent.soundSource)
				{
					audioComponent.sound = SoundManager::Instance()->PlaySound3D(audioComponent.soundSource, audioComponent.isLooped);
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

	//adTransfromSsbo->UnMapBuffer();
}

void AudioSystem::OnEnd(std::shared_ptr<Registry> registry)
{
	auto resourceManager = ResourceManager::Instance();
	auto audioPool = registry->GetComponentPool<AudioComponent>();

	if (!audioPool)
		return;

	std::for_each(std::execution::seq, audioPool->GetDenseEntitiesArray().begin(), audioPool->GetDenseEntitiesArray().end(),
		[&](const Entity& entity) -> void {
			auto& audioComponent = audioPool->GetComponent(entity);

			if (audioComponent.sound)
			{
				audioComponent.sound->stop();
				audioComponent.sound->drop();
			}
		}
	);
}

nlohmann::json AudioSystem::Serialize(Registry* registry, Entity entity)
{
	auto& audioComponent = registry->GetComponent<AudioComponent>(entity);
	nlohmann::json data;

	std::string soundPath = "none";
	for (auto& [key, soundSource] : SoundManager::Instance()->GetSoundSources())
	{
		if (soundSource == audioComponent.soundSource)
		{
			soundPath = key;
		}
	}

	data["soundSource"] = soundPath;
	data["minDist"] = audioComponent.minDist;
	data["maxDist"] = audioComponent.maxDist;
	data["volume"] = audioComponent.volume;
	data["speed"] = audioComponent.speed;
	data["isLooped"] = audioComponent.isLooped;
	data["isPaused"] = audioComponent.isPaused;

	return data;
}

void AudioSystem::DeSerialize(Registry* registry, Entity entity, const nlohmann::json& data)
{
	auto& audioComponent = registry->GetComponent<AudioComponent>(entity);

	audioComponent.soundSource = data["soundSource"] == "none" ? nullptr : SoundManager::Instance()->LoadSoundSource(data["soundSource"]);
	audioComponent.minDist = data["minDist"];
	audioComponent.maxDist = data["maxDist"];
	audioComponent.volume = data["volume"];
	audioComponent.speed = data["speed"];
	audioComponent.isLooped = data["isLooped"];
	audioComponent.isPaused = data["isPaused"];

	registry->SetFlag<AudioComponent>(entity, UPDATE_FLAG);
}