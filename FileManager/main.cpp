#include "display.h"
#include "filesystem.h"
#include "shortcutbar.h"
#include "message.h"
#include <iostream>
#include <memory>
#include <vector>
#include <list>

#ifdef _WIN32
#include <SDL.h>
#include <SDL_ttf.h>
#define MAIN() wmain()
#endif

#ifdef __unix__
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#define MAIN() main()
#endif

class Context {
public:
	bool redraw;
	bool exit;
	bool message;
	bool response;
	Message msg;
	Context() : redraw(0), exit(0), message(0), response(0), msg() {}
};

/* returns 1 for message_handling+console_message */
/* returns 2 for console_message */
int handleKeys(SDL_KeyboardEvent &e, Filesystem &dirs, Message &message) {
	switch (e.keysym.sym) {
		case SDLK_y:
			dirs.yank();
			message = Message(Message::YANK, dirs.currentDirObjName());
			return 2;
			break;
		case SDLK_p:
			dirs.paste();
			message = Message(Message::PASTE, dirs.currentDirObjName());
			return 2;
			break;
		case SDLK_d:
			message = Message(Message::REMOVE, dirs.currentDirObjName());
			return 1;
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
			message = Message(Message::QUIT);
			return 1;
			break;
	}
	return 0;
}

Context handleMessageResponse(SDL_KeyboardEvent &e, Filesystem &dirs, Message msg) {
	Context ctx;
	switch (e.keysym.sym) {
	case SDLK_y:
		switch (msg.type()) {
			case Message::REMOVE:
				dirs.remove();
				ctx.redraw = true;
				break;
			case Message::QUIT:
				ctx.exit = true;
				break;
		}
		break;
	case SDLK_n:
		ctx.redraw = true;
		break;
	}
	return ctx;
}

void handleMouse(Filesystem &dirs, ShortcutBar &bar) {
	std::string path;
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	if (bar.checkClicks(mouse_x, mouse_y, path)) {
		DirObject yanked = std::move(dirs.yanked);
		dirs = Filesystem(path);
		dirs.yanked = std::move(yanked);
	}
}

Context handleInput(SDL_Event &e, Filesystem &dirs, ShortcutBar &bar) {
	Context ctx;
	switch (e.type) {
		case SDL_QUIT:
			ctx.exit = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			ctx.redraw = true;
			dirs.currentDir().last_move = Directory::NONE;
			handleMouse(dirs, bar);
			break;
		case SDL_KEYDOWN:
			ctx.redraw = true;
			int val = handleKeys(e.key, dirs, ctx.msg);
			if (val == 1) {
				ctx.message = true;
				ctx.response = true;
			} else if (val == 2) {
				ctx.message = true;
			}
	}
	return ctx;
}

int MAIN() {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		success = false;
	}
	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize TTF: " << SDL_GetError() << std::endl;
		success = false;
	}
	Filesystem dirs;
	ShortcutBar shortcut_bar;
	Display display(1024, 768);
	display.update();

	display.renderDirectory(dirs.currentDir());
	display.renderUI(shortcut_bar);
	display.update();
	SDL_Event e;

	/* There are no messages on the console that need to be handled */
	bool handle_message = false;
	Message msg;

	while (1) {
		SDL_WaitEvent(&e);
		Context ctx;
		if (handle_message) {
			ctx = handleMessageResponse(e.key, dirs, msg);
		} else {
			ctx = handleInput(e, dirs, shortcut_bar);
		}
		SDL_PollEvent(&e);
		if (ctx.redraw) {
			if (ctx.message) dirs.currentDir().last_move = Directory::NONE;
			display.renderDirectory(dirs.currentDir());
			if (ctx.message) {
				display.renderUI(shortcut_bar, ctx.msg.message());
				msg = (ctx.msg);
				if (ctx.response) handle_message = true;
			}
			else {
				display.renderUI(shortcut_bar);
				handle_message = false;
			}

			display.update();
		}
		if (ctx.exit) {
			break;
		}
	}

	/* TODO:
	This these quits throw an excpetion since display isn't out of scope when they are called
	-> window and renderer and font arent out of scope

	TTF_Quit();
	SDL_Quit();
	*/
	return 0;
}
