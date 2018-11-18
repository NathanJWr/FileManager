#include "display.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <memory>
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
	//SDL_SetRenderDrawColor(renderer, 0,0,0,0);
	SDL_RenderClear(renderer);
}
void Display::renderTexture(SDL_Texture* texture,
    												SDL_Rect* source,
    												SDL_Rect* destination) {
  SDL_RenderCopy(renderer, texture, source, destination);
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

SDL_Texture* Display::createTextTexture(std::string text,
    																		SDL_Color color) {
  
  SDL_Surface* surface = nullptr;
  SDL_Texture* texture = nullptr;
  surface = TTF_RenderText_Solid(font, text.c_str(), color);

  if (!surface) {
		std::cerr << "Text Render Error: " << TTF_GetError() << std::endl;
    return nullptr;
  }

  texture = surfaceToTexture(surface);
  return texture;
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
  font = TTF_OpenFont("assets/Ubuntu.ttf", 90);

  if(font == NULL) {
		std::cerr << "Failed to load font: " <<  SDL_GetError() << std::endl;
    success = false;
  }
	if (!success) {
		std::cerr << "Unable to initialize SDL" << std::endl;
		exit(1);
	}
}
