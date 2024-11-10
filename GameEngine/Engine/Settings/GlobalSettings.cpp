#include "GlobalSettings.h"

float GlobalSettings::bias = 0.005;

bool GlobalSettings::Inited = false;

bool GlobalSettings::HideCursor = false;
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

std::string GlobalSettings::DefaultPath = "C:/Users/User/Desktop/GameEngine/GameEngine";
std::string GlobalSettings::DefaultScriptPath = DefaultPath + "/Scripts";

//std::string GlobalSettings::ProjectPath = "";
std::string GlobalSettings::CompilerPath = "";
std::string GlobalSettings::DefaultEnginePath = "";

std::string GlobalSettings::ProjectPath = "C:/Users/User/Desktop";
//std::string GlobalSettings::CompilerPath = "C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe";
//std::string GlobalSettings::DefaultEnginePath = "C:/Users/User/Desktop/GameEngine/GameEngine";
