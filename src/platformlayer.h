#ifdef _WIN32
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <windows.h>
#include <Lmcons.h>
#define MAIN() wmain()
#endif

#ifdef __unix__
#define MAIN() main()
#endif

#ifndef _PLATFORM_LAYER
#define _PLATFORM_LAYER
#include <string>
namespace Platform
{
    bool IsFileHidden(std::string Path, std::string Name);
    void Execute(std::string Path);
    void CMDExecute(std::string Path);
    std::string Username();
};
#endif

