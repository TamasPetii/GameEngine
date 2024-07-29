#include "SoundManager.h"

SoundManager* SoundManager::m_Instance = nullptr;

SoundManager::SoundManager()
{
	InitSoundEngine();
}

SoundManager::~SoundManager()
{
	
}

SoundManager* SoundManager::Instance()
{
	if (m_Instance == nullptr)
		m_Instance = new SoundManager();
	return m_Instance;
}

void SoundManager::Destroy()
{
	if (m_Instance != nullptr)
		delete m_Instance;
	m_Instance = nullptr;
}

void SoundManager::InitSoundEngine()
{
	m_SoundEngine = std::shared_ptr<irrklang::ISoundEngine>(irrklang::createIrrKlangDevice(), [](irrklang::ISoundEngine* ptr) -> void { if (ptr) ptr->drop(); });
}

std::shared_ptr<irrklang::ISoundSource> SoundManager::LoadSoundSource(const std::string& path)
{
	if (m_SoundSources.find(path) == m_SoundSources.end())
	{
		auto soundSource = m_SoundEngine->addSoundSourceFromFile(path.c_str());
		m_SoundSources[path] = std::shared_ptr<irrklang::ISoundSource>(soundSource);
	}
	
	return m_SoundSources[path];
}

std::shared_ptr<irrklang::ISound> SoundManager::PlaySound(std::shared_ptr<irrklang::ISoundSource> soundSource, bool isLooped)
{
	return std::shared_ptr<irrklang::ISound>(m_SoundEngine->play3D(soundSource.get(), irrklang::vec3df(0, 0, 0), isLooped, false, true), 
		[](irrklang::ISound* ptr) -> void { 
			if (ptr)
			{
				ptr->drop();
			}
		});
}

void SoundManager::SetListener(const glm::vec3& position, const glm::vec3& direction)
{
	m_SoundEngine->setListenerPosition(irrklang::vec3d(position.x, position.y, position.z), irrklang::vec3d(direction.x, direction.y, direction.z));
}