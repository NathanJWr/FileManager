#include "dirobject.h"
#include "filemanager.h"
#include "display.h"
#include "texture.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>
int main() {
	SDL_Color w = {255, 255, 255, 255};
  FileManager fmanager;
  auto list = fmanager.listCurrentDirectory();
	Display display(1024, 768);
	display.init();
	std::vector<std::unique_ptr<Texture>> tex_list;
	SDL_Delay(5000);
	for(int i = 0; i < 900; i++) {
  	for (auto n : list) {
    	std::cout << n << std::endl;
      tex_list.push_back(std::make_unique<Texture>(display.createTextTexture(n.name, w)));
  	}
	}
	SDL_Rect dest = {0, 0, 70, 20};
	for (unsigned int i = 0; i < tex_list.size(); i++) {
		display.renderTexture(tex_list[i]->get(), nullptr, &dest);
		dest.y += 20;
	}
	tex_list.clear();
	display.update();
	SDL_Delay(5000);
  
	display.update();
	SDL_Delay(5000);
  return 0;
}
