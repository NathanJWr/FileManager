#include "texture.h"
#include <SDL2/SDL.h>
#include <iostream>
Texture::Texture(SDL_Texture* tex, SDL_Rect pos) {
  texture = tex;
  position = pos;
}

Texture::~Texture() {
  SDL_DestroyTexture(texture);
}
