#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "dirobject.h"
#include "texture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
class Display {
 public:
  Display(unsigned int width, unsigned int height);
	~Display();

  void init();
  void renderDirectory(const std::vector<DirObject> &list);
	void update();
  std::unique_ptr<Texture> createTextTexture(std::string text,
                                             SDL_Color color,
                                             int x,
                                             int y);
 private:
	SDL_Texture* surfaceToTexture(SDL_Surface* surf);
	SDL_Texture* surfaceToTextureSafe(SDL_Surface* surf);
  void renderTexture(Texture*);

  SDL_Window* window;
  SDL_Renderer* renderer;
  TTF_Font* font;
  unsigned int SCREEN_W;
  unsigned int SCREEN_H;
};
#endif // DISPLAY_H_
