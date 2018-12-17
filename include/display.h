#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "dirobject.h"
#include "directory.h"
#include "texture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
class Display {
 public:
  Display(int width, int height);
  ~Display();

  bool init();
  void renderDirectory(const Directory&);
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
  int SCREEN_W;
  int SCREEN_H;
};
#endif // DISPLAY_H_
