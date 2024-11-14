#include "GlobalSettings.h"

float GlobalSettings::bias = 0.005;
bool GlobalSettings::Inited = false;

bool GlobalSettings::HideCursor = false;
bool GlobalSettings::GameViewActive = false;
bool GlobalSettings::UseDockingSpace = true;
bool GlobalSettings::EnablePhysicsInEditor = false;
bool GlobalSettings::IsViewPortActive = false;

std::string GlobalSettings::ProjectPath = "";
std::string GlobalSettings::CompilerPath = "";
std::string GlobalSettings::DefaultEnginePath = "";

//std::string GlobalSettings::CompilerPath = "C:/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/Bin/MSBuild.exe";
