#include "display.h"
#include "filesystem.h"
#include "shortcutbar.h"
#include <iostream>
#include <SDL.h>
#include <memory>
#include <vector>
#include <list>
class Context {
public:
	bool redraw;
	bool exit;
	Context() : redraw(0), exit(0) {}
};
Context handleInput(Filesystem &dirs, ShortcutBar &bar) {
	SDL_Event e;
	Context ctx;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			ctx.exit = true;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			int mouse_x, mouse_y;
			SDL_GetMouseState(&mouse_x, &mouse_y);
			bar.checkClicks(mouse_x, mouse_y);
			std::cout << bar.get_s()[0].highlighted << std::endl;
			ctx.redraw = true;
		}
		if (e.type == SDL_KEYDOWN) {
			ctx.redraw = true;
			switch (e.key.keysym.sym) {
			case SDLK_j:
				//move down in same directory
				dirs.currentDir().moveSelectedDown();
				break;
			case SDLK_k:
				//move up in same directory
				dirs.currentDir().moveSelectedUp();
				break;
			case SDLK_l:
				//move into folder or open file
				dirs.forward();
				break;
			case SDLK_h:
				//move back a directory
				dirs.back();
				break;
			case SDLK_a:
				dirs.toggleSortAlphabetically();
				break;
			case SDLK_q:
				//quit
				ctx.exit = true;
				break;
			}
		}
	}
	return ctx;
}

int wmain(){
	Filesystem dirs;
	ShortcutBar shortcut_bar;
	Display display(800, 600);
	
	display.buildShortcuts(shortcut_bar);
	display.renderDirectory(dirs.currentDir());
	display.renderUI(shortcut_bar);
	display.update();
	while (1) {
		Context ctx = handleInput(dirs, shortcut_bar);
		if (ctx.redraw) {
			display.renderDirectory(dirs.currentDir());
			display.renderUI(shortcut_bar);
			display.update();
		}
		if (ctx.exit) {
			break;
		}
		SDL_Delay(100);
	}
	shortcut_bar.clean();
	return 0;
}