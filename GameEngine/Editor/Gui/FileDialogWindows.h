#pragma once
#include <windows.h>
#include <shobjidl.h> 
#include <string>
#include <iostream>

#undef min
#undef max
#undef main

enum class FileDialogType
{
	ASK_FOR_FOLDER,
	ASK_FOR_FILE
};

class FileDialogWindows
{
public:
	static std::string ShowFileDialog();
};

