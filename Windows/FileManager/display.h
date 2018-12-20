#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "directory.h"
#include "texture.h"
#include <SDL.h>
#include <SDL_ttf.h>
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
	void renderUI();
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

	SDL_Rect dir_box;
	SDL_Rect text_box;
	unsigned int max_dir_objs;
	SDL_Color white{ 255, 255, 255, 255 };
};
#endif // DISPLAY_H_


