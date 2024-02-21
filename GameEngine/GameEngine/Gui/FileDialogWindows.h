#pragma once
#include <windows.h>
#include <shobjidl.h> 
#include <string>
#include <iostream>


#undef min
#undef max
#undef main

class FileDialogWindows
{
public:
	static std::string ShowFileDialog();
};

