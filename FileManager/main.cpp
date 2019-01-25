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
	bool shell_input;
	Message msg;
	Context() : redraw(0), exit(0), message(0), response(0), shell_input(0), msg() {}
};

enum HandleType {NONE, MESSAGE_CONSOLE, CONSOLE, SHELL};

HandleType handleKeys(SDL_KeyboardEvent &e, Filesystem &dirs, Message &message)
{
	switch (e.keysym.sym)
 	{
		case SDLK_y:
			dirs.yank();
			message = Message(Message::YANK, dirs.currentDirObjName());
			return CONSOLE;
			break;
		case SDLK_p:
			dirs.paste();
			message = Message(Message::PASTE, dirs.currentDirObjName());
			return CONSOLE;
			break;
		case SDLK_d:
			message = Message(Message::REMOVE, dirs.currentDirObjName());
			return CONSOLE;
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
		case SDLK_s:
			return SHELL;
			break;
		case SDLK_q:
			message = Message(Message::QUIT);
			return MESSAGE_CONSOLE;
			break;
		default:
			dirs.currentDir().last_move = Directory::NONE;
			break;
	}
	return NONE;
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

void handleMouse(Filesystem &dirs, ShortcutBar &bar)
{
	std::string path;
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	if (bar.checkClicks(mouse_x, mouse_y, path))
 	{
		DirObject yanked = std::move(dirs.yanked);
		dirs = Filesystem(path);
		dirs.yanked = std::move(yanked);
	}
}

Context handleInput(SDL_Event &e, Filesystem &dirs, ShortcutBar &bar)
{
	Context ctx;
	switch (e.type)
 	{
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
			HandleType val = handleKeys(e.key, dirs, ctx.msg);
			if (val == MESSAGE_CONSOLE)
		 	{
				ctx.message = true;
				ctx.response = true;
				dirs.currentDir().last_move = Directory::NONE;
			}
		 	else if (val == CONSOLE)
		 	{
				ctx.message = true;
				dirs.currentDir().last_move = Directory::NONE;
			}
			else if (val == SHELL)
			{
				ctx.shell_input = true;
			}
	}
	return ctx;
}

void drawAll(Display& display,
	 	Filesystem& dirs,
	 	ShortcutBar& shortcut_bar,
	 	Context ctx,
	 	bool new_message)
{
	display.renderDirectory(dirs.currentDir());
	if (ctx.message)
	{
		dirs.currentDir().last_move = Directory::NONE;
		display.renderUI(shortcut_bar, ctx.msg.message(), new_message);
	}
 	else
 	{
		display.renderUI(shortcut_bar);
	}

	display.update();
}

char to_lowercase(char c)
{
	if (c >= 'A' && c <= 'Z')
		return c + 32;

	return c;
}

std::string getKeyPress(SDL_Event& e)
{
	const uint8_t *state = SDL_GetKeyboardState(NULL);
	std::string tmp;
	std::string key;
	tmp = SDL_GetKeyName(e.key.keysym.sym);
	if (state[SDL_SCANCODE_LSHIFT] && e.key.keysym.sym != SDLK_LSHIFT)
	{
		key = tmp;
	}
	if (!state[SDL_SCANCODE_LSHIFT]) // Change to lowercase
	{
		for (char& c : tmp)
		{
		c = to_lowercase(c);
		}
		key = tmp;
	}
	if (state[SDL_SCANCODE_SPACE])
	{
		key = " ";
	}
	return key;
}

void handleShellInput(SDL_Event& e,
	 	Display& display,
	 	Filesystem& dirs,
	 	ShortcutBar& bar)
{
	Context shell;
	std::string command;
	while (shell.msg.message() != "return")
	{
		SDL_WaitEvent(&e);
		shell.message = true;
		if (e.type == SDL_KEYDOWN)
		{
			shell.msg = Message(getKeyPress(e));
			if (!shell.msg.message().empty()
					&& shell.msg.message() != "shift"
					&& shell.msg.message() != "return")
			{
				if (shell.msg.message() == "backspace")
				{
					display.popShellLetter();
					if (command.size() > 0) command.pop_back();
					drawAll(display, dirs, bar, shell, false);
					continue;
				}
				drawAll(display, dirs, bar, shell, false);
				command += shell.msg.message();
				std::cout << command << std::endl;
			}
		}
		SDL_PollEvent(&e);
	}
	std::cout << "Executing: " << command << std::endl;

	FILE* file = popen(command.c_str(), "r");
	pclose(file);
	display.clearShellLetters();
	drawAll(display, dirs, bar, shell, false);
}

int MAIN()
{
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
 	{
		success = false;
	}
	if (TTF_Init() == -1)
 	{
		std::cerr << "Failed to initialize TTF: " << SDL_GetError() << std::endl;
		success = false;
	}
	Filesystem dirs;
	ShortcutBar shortcut_bar;
	Display display(1280, 800);

	/* initial render */
	display.update();
	display.renderDirectory(dirs.currentDir());
	display.renderUI(shortcut_bar);
	display.update();

	SDL_Event e;
	/* There are no messages on the console that need to be handled */
	bool handle_message = false;
	Message msg; // The message sent to messageHandler

	while (1)
	{
		Context ctx;
		SDL_WaitEvent(&e);

		if (handle_message)
		{
			ctx = handleMessageResponse(e.key, dirs, msg);
		}
		else
	 	{
			ctx = handleInput(e, dirs, shortcut_bar);
		}

		if (ctx.shell_input)
		{
				handleShellInput(e, display, dirs, shortcut_bar);
		}

		if (ctx.redraw)
	 	{
			if (ctx.message) msg = ctx.msg; // This msg is sent to handleMessageRespnse
			if (ctx.response) handle_message = true; // Need to be set to handle message response
			else handle_message = false;
			std::cout << "Message: " << msg.message() << std::endl;
			drawAll(display, dirs, shortcut_bar, ctx, true);
		}

		if (ctx.exit)
	 	{
			break;
		}
		SDL_PollEvent(&e);
	}

	/* TODO:
	This these quits throw an excpetion since display isn't out of scope when they are called
	-> window and renderer and font arent out of scope

	TTF_Quit();
	SDL_Quit();
	*/
	return 0;
}
