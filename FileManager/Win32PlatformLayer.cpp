#ifdef _WIN32
#include "platformlayer.h"
bool Platform::IsFileHidden(std::string Path, std::string Name)
{
	return (GetFileAttributes(Path.c_str()) & FILE_ATTRIBUTE_HIDDEN || Name.at(0) == '.' || Name.at(0) == '_');
}

void Platform::Execute(std::string Path)
{
	ShellExecute(0, 0, Path.c_str(), 0, 0, SW_SHOW);
}

void Platform::CMDExecute(std::string Path)
{
	std::string argument = "/C " + Path;
	ShellExecute(NULL, "open", "cmd.exe", argument.c_str(), NULL, SW_SHOWNORMAL);
}

std::string Platform::Username()
{
	TCHAR username[UNLEN + 1];
	DWORD size = UNLEN + 1;
	GetUserName((TCHAR*)username, &size);
	return static_cast<std::string>(username);
}
#endif
