#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <memory>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <deque>
#include <experimental/filesystem>
#include <fstream>
#include <stack>

#include "config.h"
#include "platformlayer.h"
#include "LinuxPlatformLayer.cpp"

#include "message.cpp"
#include "context.h"

#include "dirobject.cpp"
#include "directory.cpp"

#include "filemanager.cpp"
#include "filesystem.cpp"

/* These files require SDL2 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "shortcut.cpp"
#include "shortcutbar.cpp"
#include "sdl2wrapper.h"
#include "display.cpp"
#include "input.cpp"

#include "main.cpp"
