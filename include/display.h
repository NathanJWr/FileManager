#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "texture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <memory>
#include <iostream>
class Display {
 public:
  Display(unsigned int width, unsigned int height);
	~Display();
	void update();
	//void renderTexture(SDL_Texture*, SDL_Rect*, SDL_Rect*);
  void renderTexture(Texture*);
  std::unique_ptr<Texture> createTextTexture(std::string text, SDL_Color color);
  void init();
 private:
	SDL_Texture* surfaceToTexture(SDL_Surface* surf);
	SDL_Texture* surfaceToTextureSafe(SDL_Surface* surf);

  SDL_Window* window;
  SDL_Renderer* renderer;
  TTF_Font* font;
  unsigned int SCREEN_W;
  unsigned int SCREEN_H;
};
#endif // DISPLAY_H_
