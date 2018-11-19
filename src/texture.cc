#include "texture.h"
#include <SDL2/SDL.h>
#include <iostream>
Texture::Texture(SDL_Texture* tex, SDL_Rect pos) {
  texture = tex;
  position = pos;
}

Texture::~Texture() {
	std::cout << "Texture Destructor Called" << std::endl;
	SDL_DestroyTexture(texture);
}

SDL_Texture* Texture::get_t() {
	return texture;
}

SDL_Rect* Texture::get_p() {
  return &position;
}
