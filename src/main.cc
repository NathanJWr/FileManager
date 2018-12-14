#include "filemanager.h"
#include "display.h"
#include "filesystem.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <list>
int handleInput(Filesystem &dirs) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_j:
          //move down in same directory
          dirs.currentDir().moveSelectedDown();
          break;
        case SDLK_k:
          //move up in same directory
          dirs.currentDir().moveSelectedUp();
          break;
        case SDLK_l:
          //move into folder or open file
					dirs.forward();
          break;
        case SDLK_h:
          //move back a directory
					dirs.back();
          break;
        case SDLK_q:
          //quit
          exit(0);
          break;
      }
      return 1;
    }
  }
  return 0;
}

int main() {
  Display display(1024, 768);
  if (!display.init()) {
    std::cerr << "Failed to initialize Display!" << std::endl;
	exit(1);
  }
  Filesystem dirs;
  while (1) {
    if (handleInput(dirs) == 1) {
      display.renderDirectory(dirs.currentDir());
      display.update();
  	}
		SDL_Delay(100);
  }
  return 0;
}
