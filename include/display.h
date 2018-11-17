#ifndef DISPLAY_H_
#define DISPLAY_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
class Display {
 public:
  Display(unsigned int width, unsigned int height);
  void init();
 private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  TTF_Font* font;
  unsigned int SCREEN_W;
  unsigned int SCREEN_H;
};
#endif // DISPLAY_H_
