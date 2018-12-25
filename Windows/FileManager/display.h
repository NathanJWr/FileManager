#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "directory.h"
#include "texture.h"
#include "shortcutbar.h"
#include "shortcut.h"
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
	void renderDirectory(Directory&);
	void renderUI(ShortcutBar &bar);
	void buildDirectory(Directory& dir);
	void buildShortcut(Shortcut& shortcut);
	void buildShortcuts(ShortcutBar &bar);
	void update();
	SDL_Texture* createTextTexture(std::string text,
		SDL_Color color,
		SDL_Rect &pos);
private:
	SDL_Texture* surfaceToTexture(SDL_Surface* surf);
	SDL_Texture* surfaceToTextureSafe(SDL_Surface* surf);
	void renderTexture(SDL_Texture*, SDL_Rect);
	void renderCurrentPath();
	void renderShortcuts(ShortcutBar &bar);

	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	int SCREEN_W;
	int SCREEN_H;

	SDL_Rect dir_box;
	SDL_Rect shortcut_box;
	SDL_Rect text_box;
	std::string cur_path;
	unsigned int max_dir_objs;
	SDL_Color white{ 255, 255, 255, 255 };
	SDL_Color yellow{ 255, 255, 153, 0 };
	SDL_Color green = { 0, 255, 0, 255 };
	SDL_Color blue = { 0, 50, 255, 255 };
};
#endif // DISPLAY_H_


