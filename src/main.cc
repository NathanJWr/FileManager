#include "dirobject.h"
#include "filemanager.h"
#include "display.h"
#include "texture.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
void handleInput(Directory &dir) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_j:
          //move down in same directory
          dir.moveSelectedDown();
          break;
        case SDLK_k:
          //move up in same directory
          dir.moveSelectedUp();
          break;
        case SDLK_l:
          //move into folder or open file
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
  FileManager fmanager;
	Display display(1024, 768);
	display.init();
  Directory dir = fmanager.listCurrentDirectory();
  while (1) {
    handleInput(dir);
    display.renderDirectory(dir);
	  display.update();
    SDL_Delay(100);
  }

  return 0;
}
