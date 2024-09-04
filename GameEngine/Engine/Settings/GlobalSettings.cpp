#include "GlobalSettings.h"

bool GlobalSettings::GameViewActive = false;
bool GlobalSettings::UseDockingSpace = true;
bool GlobalSettings::EnablePhysicsInEditor = false;
bool GlobalSettings::EnableAnimationInEditor = false;

bool GlobalSettings::UseSkybox = true;
glm::vec3 GlobalSettings::SkyboxRotation = glm::vec3(0.f);
glm::vec3 GlobalSettings::SkyboxRotationSpeed = glm::vec3(0.f);
glm::ivec3 GlobalSettings::SkyboxRotationDirection = glm::ivec3(0);
glm::mat4 GlobalSettings::SkyboxRotationMatrix = glm::mat4(1);
std::shared_ptr<TextureGL> GlobalSettings::SkyboxTexture = nullptr;