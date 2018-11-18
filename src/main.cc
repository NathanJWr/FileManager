#include "dirobject.h"
#include "filemanager.h"
#include "display.h"
#include <iostream>
#include <SDL2/SDL.h>
int main() {
	SDL_Color w = {255, 255, 255, 255};
  FileManager fmanager;
  auto list = fmanager.listCurrentDirectory();
	Display display(1024, 768);
	display.init();
	std::vector<SDL_Texture*> tex_list;
  for (auto n : list) {
    std::cout << n << std::endl;
		tex_list.push_back(display.createTextTexture(n.name, w));
  }
	SDL_Rect dest = {0, 0, 70, 20};
	for (auto t : tex_list) {
		display.renderTexture(t, nullptr, &dest);
		dest.y += 20;
	}
	display.update();
	SDL_Delay(5000);
  return 0;

}
