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
	std::shared_ptr<irrklang::ISound> PlaySound(std::shared_ptr<irrklang::ISoundSource> soundSource);
private:
	SoundManager();
	void InitSoundEngine();
	static SoundManager* m_Instance;
	std::unique_ptr<irrklang::ISoundEngine> m_SoundEngine;
	std::unordered_map<std::string, std::shared_ptr<irrklang::ISoundSource>> m_SoundSources;
};

/*
static irrklang::ISoundEngine* m_audioEngine = irrklang::createIrrKlangDevice();
static irrklang::ISoundSource* m_pistolSource = m_audioEngine->addSoundSourceFromFile("../Assets/Awp.wav");
static irrklang::ISound* sound = m_audioEngine->play3D(m_pistolSource, irrklang::vec3df(0, 0, 0), false, false, true);
*/