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
	bool message;
	std::string text;
	Context() : redraw(0), exit(0), message(0) {}
};

/* returns 1 for exit */
int handleKeys(SDL_KeyboardEvent &e, Filesystem &dirs, std::string &message) {
	switch (e.keysym.sym) {
		case SDLK_y:
			dirs.yank();
			break;
		case SDLK_p:
			dirs.paste();
			break;
		case SDLK_d:
			message = "Are you sure you want to delete " + dirs.currentDirObjName() + "? Y/N";
			break;
		case SDLK_j:
			dirs.currentDir().moveSelectedDown();
			break;
		case SDLK_k:
			dirs.currentDir().moveSelectedUp();
			break;
		case SDLK_l:
			dirs.forward();
			break;
		case SDLK_h:
			dirs.back();
			break;
		case SDLK_a:
			dirs.toggleSortAlphabetically();
			break;
		case SDLK_q:
			return 1;
	}
	return 0;
}

void handleMessageResponse(SDL_KeyboardEvent &e, Filesystem &dirs) {
	switch (e.keysym.sym) {
		case SDLK_y:
			dirs.remove();
			break;
		case SDLK_n:
			// Do nothing
			break;
	}
}

void handleMouse(Filesystem &dirs, ShortcutBar &bar) {
	std::string path;
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	if (bar.checkClicks(mouse_x, mouse_y, path)) {
		DirObject yanked = dirs.yanked;
		dirs.clean();
		dirs = Filesystem(path);
		dirs.yanked = yanked;
	}
}

Context handleInput(SDL_Event &e, Filesystem &dirs, ShortcutBar &bar, bool message) {
	Context ctx;
	switch (e.type) {
		case SDL_QUIT:
			ctx.exit = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			ctx.redraw = true;
			handleMouse(dirs, bar);
			break;
		case SDL_KEYDOWN:
			ctx.redraw = true;
			std::string msg = "";
			if (message) {
				handleMessageResponse(e.key, dirs);
			} else if (handleKeys(e.key, dirs, msg) == 1) {
				ctx.exit = true;
			}
			if (!msg.empty()) {
				ctx.message = true;
				ctx.text = msg;
			}
	}
	return ctx;
}

int wmain(){
	Filesystem dirs;
	ShortcutBar shortcut_bar;
	Display display(800, 600);
	
	display.buildShortcuts(shortcut_bar);
	display.buildDirectory(dirs.currentDir());
	display.renderDirectory(dirs.currentDir());
	display.renderUI(shortcut_bar);
	display.update();

	SDL_Event e;
	bool message = false;
	while (1) {
		SDL_WaitEvent(&e);
		Context ctx = handleInput(e, dirs, shortcut_bar, message);
		SDL_PollEvent(&e);
		if (ctx.redraw) {
			display.buildDirectory(dirs.currentDir());
			display.renderDirectory(dirs.currentDir());

			if (ctx.message) {
				display.renderUI(shortcut_bar, ctx.text);
				message = true;
			} else {
				display.renderUI(shortcut_bar);
				message = false;
			}

			display.update();
		}
		if (ctx.exit) {
			break;
		}
	}
	shortcut_bar.clean();
	return 0;
}
