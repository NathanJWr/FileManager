#include "texture.h"
#include <SDL2/SDL.h>
#include <iostream>
Texture::Texture(SDL_Texture* tex) : texture(tex) {

}

Texture::~Texture() {
	std::cout << "Texture Destructor Called" << std::endl;
	SDL_DestroyTexture(texture);
}

SDL_Texture* Texture::get() {
	return texture;
}
