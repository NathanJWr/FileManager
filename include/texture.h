#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <SDL2/SDL.h>
class Texture {
 public:
	Texture(SDL_Texture*);
	~Texture();
	SDL_Texture* get();
 private:
	SDL_Texture* texture;
};
#endif // TEXTURE_H_
