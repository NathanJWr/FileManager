#include "Win32PlatformLayer.h"
#include <windows.h>
bool IsFileHidden(std::string Path, std::string Name)
{
	return (GetFileAttributes(Path.c_str()) & FILE_ATTRIBUTE_HIDDEN || Name.at(0) == '.' || Name.at(0) == '_');
}

void Execute(std::string Path)
{
	ShellExecute(0, 0, Path.c_str(), 0, 0, SW_SHOW);
}
