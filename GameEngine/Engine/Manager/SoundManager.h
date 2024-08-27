#pragma once
#include "EngineApi.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <Model/Model.h>
#include <IrrKlang/irrKlang.h>

class ENGINE_API SoundManager
{
public:
	~SoundManager();
	static SoundManager* Instance();
	static void Destroy();
	std::shared_ptr<irrklang::ISoundSource> LoadSoundSource(const std::string& path);
	std::shared_ptr<irrklang::ISound> PlaySound(std::shared_ptr<irrklang::ISoundSource> soundSource, bool isLooped = false);
	void PlaySound2D(std::shared_ptr<irrklang::ISoundSource> soundSource);
	void SetListener(const glm::vec3& position, const glm::vec3& direction);

	auto GetSoundEngine() { return m_SoundEngine; }
	auto& GetSoundSources() { return m_SoundSources; }
private:
	SoundManager();
	void InitSoundEngine();
	static SoundManager* m_Instance;
	std::shared_ptr<irrklang::ISoundEngine> m_SoundEngine;
	std::unordered_map<std::string, std::shared_ptr<irrklang::ISoundSource>> m_SoundSources;
};