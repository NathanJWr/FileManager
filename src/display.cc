#include "display.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
Display::Display(unsigned int width, unsigned int height) :
  SCREEN_W(width),
  SCREEN_H(height) {
}
void Display::init() {
  bool success = true;
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    success = false;
  }
  else if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    std::cerr << "Linear filtering not enabled!" << std::endl;
  }
  window = SDL_CreateWindow("Game",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      SCREEN_W,
      SCREEN_H,
      SDL_WINDOW_SHOWN);
  if(window == NULL) {
    success = false;
  }
  else {
    renderer = SDL_CreateRenderer(window,
        -1,
        SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
      success = false;
    }
    else {
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
  }

  if(TTF_Init() == -1) {
    printf("Failed to initialize TTF: %s\n", SDL_GetError());
    success = false;
  }
  font = TTF_OpenFont("assets/fonts/Ubuntu.ttf", 90);

  if(font == NULL) {
    printf("Failed to load font: %s\n", SDL_GetError());
    success = false;
  }
}
