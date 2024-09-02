#include "GlobalSettings.h"

bool GlobalSettings::GameViewActive = false;
bool GlobalSettings::UseDockingSpace = true;
bool GlobalSettings::UseSkybox = true;
std::shared_ptr<TextureGL> GlobalSettings::SkyboxTexture = nullptr;