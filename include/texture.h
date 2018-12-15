#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <SDL2/SDL.h>
class Texture {
 public:
  Texture(SDL_Texture*, SDL_Rect);
  ~Texture();
  SDL_Texture* get_t();
  SDL_Rect* get_p();
  Texture(const Texture&) = delete; // Deletes the copy constructor
 private:
  SDL_Texture* texture;
  SDL_Rect position;
};
#endif // TEXTURE_H_
