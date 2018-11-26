#include "filemanager.h"
#include "display.h"
#include "filesystem.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <list>
void handleInput(Filesystem &dirs) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_j:
          //move down in same directory
          dirs.getCurrent().moveSelectedDown();
          break;
        case SDLK_k:
          //move up in same directory
          dirs.getCurrent().moveSelectedUp();
          break;
        case SDLK_l:
          //move into folder or open file
          //dir.moveRight();
          break;
        case SDLK_h:
          //move back a directory
          break;
        case SDLK_q:
          //quit
          exit(0);
          break;
      }
    }
  }
}

int main() {
	Display display(1024, 768);
  std::list<Directory> directories;
	display.init();
  //Directory dir = fmanager.listCurrentDirectory();
  Filesystem dirs;
  while (1) {
    handleInput(dirs);
    display.renderDirectory(dirs.getCurrent());
	  display.update();
    SDL_Delay(100);
  }

  return 0;
}
