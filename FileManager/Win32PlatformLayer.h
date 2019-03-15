#ifdef _WIN32
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <windows.h>
#include <Lmcons.h>
#define EXECUTE(Path) std::string tmp = "/C " + Path; ShellExecute(NULL, "open", "cmd.exe", tmp.c_str(), NULL, SW_SHOWNORMAL);
#define USERNAME() \
	TCHAR username[UNLEN + 1]; \
	DWORD size = UNLEN + 1; \
	GetUserName((TCHAR*)username, &size);
#define MAIN() wmain()
#ifndef _WIN32PLATFORM
#define _WIN32PLATFORM
#include <string>
bool IsFileHidden(std::string Path, std::string Name);
void Execute(std::string Path);
#endif
#endif

