#pragma once
#include <windows.h>
#include <shobjidl.h> 
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#undef min
#undef max
#undef main

enum class FileDialogType
{
	SAVE_DIALOG,
	OPEN_DIALOG
};

enum class FileDialogReturnType
{
	PICK_FILE,
	PICK_FOLDER
};

struct FileDialogOption
{
	FileDialogType dialogType{ FileDialogType::SAVE_DIALOG };
	FileDialogReturnType returnType{ FileDialogReturnType::PICK_FOLDER };
	std::vector<std::pair<std::wstring, std::wstring>> filters;
};

class FileDialogWindows
{
public:
	static std::string ShowFileDialog(const FileDialogOption& option);
};
