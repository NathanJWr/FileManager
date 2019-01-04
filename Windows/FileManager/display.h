#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "sdl2wrapper.h"
#include "directory.h"
#include "shortcutbar.h"
#include "shortcut.h"
#include <string>
#include <deque>
class Display {
public:
	Display(int width, int height);
	/* put the Directory contentts on screen */
	void renderDirectory(Directory&);
	/* put all of the UI elements on the screen */
	void renderUI(ShortcutBar &bar, std::string message);
	void renderUI(ShortcutBar &bar);
	void renderConsoleMessage(std::string message);
	/* display the screen */
	void update();
	
private:
	void renderTextTexture(SDL2::TextTexture& tex);
	void renderCurrentPath();
	void renderShortcuts(ShortcutBar &bar);
	SDL_Color determineColor(DirObject obj);

	SDL2::window_ptr window;
	SDL2::renderer_ptr renderer;
	SDL2::font_ptr font;

	SDL_Color white{ 255, 255, 255, 255 };
	SDL_Color yellow{ 255, 255, 153, 0 };
	SDL_Color green = { 0, 255, 0, 255 };
	SDL_Color blue = { 0, 50, 255, 255 };
	
	std::deque<SDL2::TextTexture> DirTextures;
	std::vector<SDL2::TextTexture> shortcuts;
	
	int SCREEN_W;
	int SCREEN_H;

	SDL_Rect dir_box;
	SDL_Rect shortcut_box;
	SDL_Rect text_box;
	SDL_Rect console_box;

	std::string cur_path;


};
#endif // DISPLAY_H_


