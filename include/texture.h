#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <SDL2/SDL.h>
class Texture {
 public:
	Texture(SDL_Texture*);
	~Texture();
	SDL_Texture* get();
  Texture(const Texture&) = delete; // Deletes the copy constructor
 private:
	SDL_Texture* texture;
};
#endif // TEXTURE_H_
