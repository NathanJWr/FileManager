#include "display.h"
#include "config.h"
Display::Display(int width, int height) : 
	/* These need to be in the initializer list since they don't have default constructors */
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

void Display::renderDirectory(Directory& dir) {
	DirTextures.clear(); // TODO: find a way to not have to clear entirely
	cur_path = dir.path();
	auto list = dir.get();
	if (list.empty()) {
		return;
	}
	unsigned int selected_pos = 0;
	unsigned int start_pos = 0;
	unsigned int end_pos = 0;
	for (unsigned int i = 0; i < list.size(); i++) {
		DirObject& n = list[i];
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

	// Build all the textures for now
	for (auto& n : list) {
		SDL_Color color;
		if (n.selected) {
			color = white;
		}
		else if (n.type() == DirObject::FILE) {
			color = green;
		}
		else if (n.type() == DirObject::FOLDER) {
			color = blue;
		}
		DirTextures.emplace_back(SDL2::makeTextTexture(font, n.name().c_str(), color, renderer));
		DirTextures.back().pos.x = dir_box.x;
	}
	int y = dir_box.y, x = dir_box.x;

	// Display textures
	for (unsigned int i = start_pos; i < end_pos; i++) {
		auto& n = DirTextures[i];
		
		if (y < dir_box.y + dir_box.h - 20) {

			n.pos.y = y;
			n.pos.x = dir_box.x;
			y += text_box.h;
			renderTextTexture(n);
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
	SDL2::TextTexture text = SDL2::makeTextTexture(font, message.c_str(), white, renderer);
	text.pos.x = console_box.x;
	text.pos.y = console_box.y;
	renderTextTexture(text);
}

void Display::renderShortcuts(ShortcutBar &bar) {
	SDL_SetRenderDrawColor(renderer.get(), 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer.get(), &shortcut_box);
	int buf = 0;
	auto &list = bar.get_s();
	for (auto& n : list) {
		SDL_Color color;
		if (n.highlighted) {
			color = yellow;
		} else {
			color = white;
		}
		SDL2::TextTexture text = SDL2::makeTextTexture(font, n.name().c_str(), color, renderer);
		text.pos.y = buf;
		buf += text_box.h;
		n.pos = text.pos;
		renderTextTexture(text);
	}
}

void Display::renderCurrentPath() {
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
	auto p = SDL2::makeTextTexture(font, buf.c_str(), white, renderer);
	p.pos.x = dir_box.x;
	p.pos.y = dir_box.y - text_box.h;
	renderTextTexture(p);
}

void Display::renderTextTexture(SDL2::TextTexture& tex) {
	SDL_RenderCopy(renderer.get(), tex.texture.get(), NULL, &tex.pos);
}
