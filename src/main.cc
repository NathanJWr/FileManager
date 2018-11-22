#include "dirobject.h"
#include "filemanager.h"
#include "display.h"
#include "texture.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
int main() {
  FileManager fmanager;
	Display display(1024, 768);
	display.init();
  std::vector<DirObject> list = fmanager.listCurrentDirectory();
  display.renderDirectory(list);
	display.update();
  SDL_Delay(5000);
  return 0;
}
