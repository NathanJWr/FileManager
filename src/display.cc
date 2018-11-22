#include "display.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
Display::Display(unsigned int width, unsigned int height) :
  SCREEN_W(width),
  SCREEN_H(height) {
    window = nullptr;
    renderer = nullptr;
    font = nullptr;
}

Display::~Display() {
  std::cout << "Display being destroyed\n";
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}

void Display::update() {
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
}

void Display::renderDirectory(const std::vector<DirObject> &list) {
  std::vector<std::unique_ptr<Texture>> tex_list;
  int y = 0, x = 0;
  SDL_Color white = {255, 255, 255};
  //SDL_Color green = {25, 80, 99};
  SDL_Color blue = {0, 50, 255};
 
  SDL_Color color;
  for (auto n : list) {
    std::cout << n << std::endl;
    if (n.type == DirObject::FILE) {
      color = white;
    } else if (n.type == DirObject::FOLDER) {
      color = blue;
    } 
    tex_list.emplace_back(createTextTexture(n.name, color, x, y));
    y += 20;
  }
  for (unsigned int i = 0; i < tex_list.size(); i++) {
    renderTexture(tex_list[i].get());
  }
}

void Display::renderTexture(Texture* texture) {
  SDL_Rect* destination = texture->get_p();
  SDL_Rect* source = nullptr;
  SDL_RenderCopy(renderer, texture->get_t(), source, destination);
}

SDL_Texture* Display::surfaceToTexture(SDL_Surface* surf) {
  SDL_Texture* text = nullptr;
  text = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_FreeSurface(surf);

  if (!text) {
		std::cerr << "Texture Creation Error: " <<  SDL_GetError() << std::endl;
  }
  return text;
}

SDL_Texture* Display::surfaceToTextureSafe(SDL_Surface* surf) {
  SDL_Texture* text = nullptr;
  text = SDL_CreateTextureFromSurface(renderer, surf);

  if (!text) {
		std::cerr << "Texture Creation Error: " << SDL_GetError() << std::endl;
    return nullptr;
  }
  return text;
}

std::unique_ptr<Texture> Display::createTextTexture(std::string text,
    																		            SDL_Color color,
                                                    int x,
                                                    int y) {
  SDL_Surface* surface = nullptr;
  SDL_Texture* tex = nullptr;
  SDL_Rect pos = {0, 0, 0, 0};
  surface = TTF_RenderText_Solid(font, text.c_str(), color);
  if (!surface) {
    std::cerr << "Text creationg error" << std::endl;
  }
  pos.w = surface->w;
  pos.h = surface->h;
  pos.x = x;
  pos.y = y;

  if (!surface) {
		std::cerr << "Text Render Error: " << TTF_GetError() << std::endl;
    return nullptr;
  }

  tex = surfaceToTexture(surface);

  auto ptr = std::unique_ptr<Texture>(new Texture(tex, pos));
  return ptr;
}

void Display::init() {
  bool success = true;
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    success = false;
  }
  else if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    std::cerr << "Linear filtering not enabled!" << std::endl;
  }
  window = SDL_CreateWindow("FileManager",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      SCREEN_W,
      SCREEN_H,
      SDL_WINDOW_SHOWN);
  if(window == NULL) {
    success = false;
  }
  else {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr) {
      success = false;
    }
    else {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
  }
  if(TTF_Init() == -1) {
		std::cerr << "Failed to initialize TTF: " <<  SDL_GetError() << std::endl;

    success = false;
  }
  font = TTF_OpenFont("assets/Ubuntu.ttf", 12);

  if(font == NULL) {
		std::cerr << "Failed to load font: " <<  SDL_GetError() << std::endl;
    success = false;
  }
	if (!success) {
		std::cerr << "Unable to initialize SDL" << std::endl;
		exit(1);
	}
}
