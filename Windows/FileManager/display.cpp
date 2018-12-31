#include "display.h"
#include "config.h"
#include "sdl2wrapper.h"
#include <SDL.h>
#include <SDL_ttf.h>
Display::Display(int width, int height) :
	window(SDL2::makeWindow("FileManager", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN)),
	renderer(SDL2::makeRenderer(window, -1, SDL_RENDERER_ACCELERATED)),
	font(SDL2::makeFont("assets/Ubuntu.ttf", 20)) {

	SCREEN_W = width;
	SCREEN_H = height;
	
	int dir_x = static_cast<int>(width / 5.5);
	int dir_y = static_cast<int>(height / 5.5);
	int dir_w = SCREEN_W;
	int dir_h = static_cast<int>(height / 1.5);
	dir_box = {dir_x, dir_y, dir_w, dir_h};
	console_box = { dir_x, dir_y + dir_h, SCREEN_W, SCREEN_H};

	shortcut_box = {0, 0, dir_x, height};

	/* figure out the height and width of a text_box */
	auto temp_buff = SDL2::makeTextSurface(font, "text", white);
	text_box.w = temp_buff.get()->w;
	text_box.h = temp_buff.get()->h;
	max_dir_objs = static_cast<unsigned int>(dir_box.h / text_box.h);
}

void Display::update() {
	SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
	SDL_RenderPresent(renderer.get());
	SDL_RenderClear(renderer.get());
}
void Display::buildDirectory(Directory& dir) {
	dir.clean();
	cur_path = dir.path();
	auto& list = dir.get();
	if (list.empty()) {
		return;
	}
	int y = dir_box.y, x = dir_box.x;

	SDL_Color color;
	unsigned int selected_pos = 0;
	unsigned int start_pos = 0;
	unsigned int end_pos = 0;
	for (unsigned int i = 0; i < list.size(); i++) {
		DirObject n = list[i];
		if (n.selected) selected_pos = i;
	}
	unsigned int mid = max_dir_objs / 2;
	if (selected_pos > mid && selected_pos < list.size() - mid) {
		start_pos = selected_pos - mid;
		end_pos = selected_pos + mid;
	}
	else if (selected_pos < mid) {
		start_pos = 0;
		end_pos = static_cast<unsigned int> (list.size());
	}
	else {
		start_pos = selected_pos - mid;
		end_pos = static_cast<unsigned int> (list.size());
	}
	for (unsigned int i = start_pos; i < end_pos; i++) {
		DirObject& n = list[i];
		if (n.selected) {
			color = white;
		}
		else if (n.type() == DirObject::FILE) {
			color = green;
		}
		else if (n.type() == DirObject::FOLDER) {
			color = blue;
		}
		if (y < dir_box.y + dir_box.h - 20) {
			if (n.texture == NULL) {
				n.texture = createTextTexture(n.name(), color, n.pos);
			}
			n.pos.y = y;
			n.pos.x = dir_box.x;
			/*
			if (n.pos.x + n.pos.w > dir_box.x + dir_box.w) {
				n.texture = NULL;
			}
			*/
			y += text_box.h;
		}
	}

}

void Display::renderDirectory(Directory& dir) {
	auto list = dir.get();
	if (list.empty()) {
		return;
	}

	for (unsigned int i = 0; i < list.size(); i++) {
		if (list[i].texture != NULL) {
			renderTexture(list[i].texture, list[i].pos);
		}
	}
}

void Display::renderUI(ShortcutBar &bar) {
	SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer.get(), &dir_box);
	SDL_RenderDrawRect(renderer.get(), &console_box);
	renderShortcuts(bar);
	renderCurrentPath();
}

void Display::renderUI(ShortcutBar &bar, std::string console_message) {
	renderUI(bar);
	renderConsoleMessage(console_message);
}

void Display::renderConsoleMessage(std::string message) {
	SDL_Rect pos = console_box; // Width and Height are going to be set in the next call
	SDL_Texture* tex = createTextTexture(message, white, pos);
	renderTexture(tex, pos);
	SDL_DestroyTexture(tex);
}

void Display::renderShortcuts(ShortcutBar &bar) {
	SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer.get(), &shortcut_box);

	auto &list = bar.get_s();
	for (unsigned int i = 0; i < list.size(); i++) {
		if (list[i].redraw()) {
			buildShortcut(list[i]);
		}
		renderTexture(list[i].texture, list[i].pos);
	}
}

void Display::buildShortcut(Shortcut &shortcut) {
	SDL_Color color;
	if (shortcut.highlighted) {
		color = yellow;
	}
	else {
		color = white;
	}
	shortcut.clean();
	if (shortcut.texture == NULL) {
		shortcut.texture = createTextTexture(shortcut.name(), color, shortcut.pos);
	}
}
void Display::buildShortcuts(ShortcutBar &bar) {
	int buf = 0;
	for (unsigned int i = 0; i < bar.get_s().size(); i++) {
		if (bar.get_s()[i].texture == NULL) {
			bar.get_s()[i].texture = createTextTexture(bar.get_s()[i].name(), white, bar.get_s()[i].pos);
		}
		bar.get_s()[i].pos.y = buf;
		buf += text_box.h;
		bar.get_s()[i].pos.x = shortcut_box.x;
	}
}

void Display::renderCurrentPath() {
	int y = dir_box.y - text_box.h;
	int x = dir_box.x;
	int count = 0;
	for (unsigned int i = 0; i < cur_path.size(); i++) {
		if (cur_path.at(i) == SLASH[0]) {
			count++;
		}
	}
	std::string buff;
	if (count > 2) buff = ".." + SLASH;
	else buff = "";
	while (count > 2) {
		size_t pos = cur_path.find_first_of(SLASH);
		cur_path.erase(cur_path.begin(), cur_path.begin() + pos + 1);
		count--;
	}
	std::string buf = buff + cur_path;
	SDL_Rect pos = { x, y, text_box.w, text_box.h };
	auto p = createTextTexture(buf, white, pos);
	renderTexture(p, pos);
	SDL_DestroyTexture(p);
}

void Display::renderTexture(SDL_Texture* tex, SDL_Rect pos) {
	SDL_RenderCopy(renderer.get(), tex, NULL, &pos);
}


SDL_Texture* Display::surfaceToTexture(SDL_Surface* surf) {
	SDL_Texture* text = nullptr;
	text = SDL_CreateTextureFromSurface(renderer.get(), surf);
	SDL_FreeSurface(surf);

	if (!text) {
		std::cerr << "Texture Creation Error: " << SDL_GetError() << std::endl;
	}
	return text;
}

SDL_Texture* Display::createTextTexture(std::string text,
	SDL_Color color,
	SDL_Rect &pos) {
	SDL_Surface* surface = nullptr;
	SDL_Texture* tex = nullptr;
	surface = TTF_RenderText_Solid(font.get(), text.c_str(), color);
	if (surface == NULL) {
		std::cerr << "Text creationg error" << std::endl;
	}
	pos.w = surface->w;
	pos.h = surface->h;

	if (!surface) {
		std::cerr << "Text Render Error: " << TTF_GetError() << std::endl;
		return nullptr;
	}

	tex = surfaceToTexture(surface);
	return tex;
}
