#include "dirobject.h"
#include "filemanager.h"
#include "display.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>
int main() {
	SDL_Color w = {255, 255, 255, 255};
  FileManager fmanager;
  auto list = fmanager.listCurrentDirectory();
	Display display(1024, 768);
	display.init();
	std::vector<std::unique_ptr<SDL_Texture, sdl_deleter>> tex_list;
  for (auto n : list) {
    std::cout << n << std::endl;
		tex_list.push_back(display.createTextTexture(n.name, w));
  }
	SDL_Rect dest = {0, 0, 70, 20};
	for (unsigned int i = 0; i < tex_list.size(); i++) {
		display.renderTexture(tex_list[i].get(), nullptr, &dest);
		dest.y += 20;
	}
	display.update();
	SDL_Delay(5000);
  return 0;

}
