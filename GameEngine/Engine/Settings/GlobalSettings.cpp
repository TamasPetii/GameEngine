#include "GlobalSettings.h"

float GlobalSettings::bias = 0.005;
bool GlobalSettings::Inited = false;

bool GlobalSettings::HideCursor = false;
bool GlobalSettings::GameViewActive = false;
bool GlobalSettings::UseDockingSpace = true;
bool GlobalSettings::EnablePhysicsInEditor = false;
bool GlobalSettings::IsViewPortActive = false;
bool GlobalSettings::DeployedGame = false;
bool GlobalSettings::SimulateScene = true;

bool GlobalSettings::UseLod = true;
std::array<float, 4> GlobalSettings::LodDistances = { 40, 100, 200, 500 };
std::string GlobalSettings::ProjectPath = "";
std::string GlobalSettings::CompilerPath = "";
std::string GlobalSettings::DefaultEnginePath = "";
