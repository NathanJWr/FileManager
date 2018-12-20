#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <SDL.h>
class Texture {
public:
	Texture(SDL_Texture*, SDL_Rect);
	~Texture();
	constexpr SDL_Texture* get_t() const {
		return texture;
	}
	constexpr SDL_Rect* get_p() {
		return &position;
	}
	Texture(const Texture&) = delete; // Deletes the copy constructor
private:
	SDL_Texture* texture;
	SDL_Rect position;
};
#endif // TEXTURE_H_
