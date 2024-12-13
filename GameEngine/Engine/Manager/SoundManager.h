#pragma once
#include "EngineApi.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <IrrKlang/irrKlang.h>
#include <glm/glm.hpp>

class ENGINE_API SoundManager
{
public:
	static SoundManager* Instance();
	static void Destroy();
	std::shared_ptr<irrklang::ISoundSource> LoadSoundSource(const std::string& path);
	std::shared_ptr<irrklang::ISound> PlaySound3D(std::shared_ptr<irrklang::ISoundSource> soundSource, bool isLooped = false);
	std::shared_ptr<irrklang::ISound> PlaySound2D(std::shared_ptr<irrklang::ISoundSource> soundSource, bool isLooped = false);
	void SetListener(const glm::vec3& position, const glm::vec3& direction);

	auto GetSoundEngine() { return m_SoundEngine; }
	auto& GetSoundSources() { return m_SoundSources; }
private:
	SoundManager();
	virtual ~SoundManager();
	void InitSoundEngine();
	static SoundManager* m_Instance;
	std::shared_ptr<irrklang::ISoundEngine> m_SoundEngine;
	std::unordered_map<std::string, std::shared_ptr<irrklang::ISoundSource>> m_SoundSources;
};