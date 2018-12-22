#include "display.h"
#include "config.h"
#include <SDL.h>
#include <SDL_ttf.h>
Display::Display(int width, int height) :
	SCREEN_W(width),
	SCREEN_H(height) {
	window = nullptr;
	renderer = nullptr;
	font = nullptr;
	int dir_x = static_cast<int>(width / 5.5);
	int dir_y = static_cast<int>(height / 5.5);
	int dir_w = static_cast<int>(width / 1.5);
	int dir_h = static_cast<int>(height / 1.5);
	dir_box = {dir_x, dir_y, dir_w, dir_h};

	shortcut_box = {0, 0, dir_x, height};
	if (!init()) {
		exit(1);
	}

	/* figure out the height and width of a text_box */
	SDL_Surface* surface = TTF_RenderText_Solid(font, "n/a", white);
	text_box.w = surface->w;
	text_box.h = surface->h;
	SDL_FreeSurface(surface);
	max_dir_objs = static_cast<unsigned int>(dir_box.h / text_box.h);
}

Display::~Display() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}

void Display::update() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
	SDL_RenderClear(renderer);
}

void Display::renderDirectory(const Directory& dir) {
	cur_path = dir.path;
	std::vector<std::unique_ptr<Texture>> tex_list;
	auto list = dir.get();
	if (list.empty()) {
		return;
	}
	int y = dir_box.y, x = dir_box.x;
	SDL_Color green = { 0, 255, 0, 255 };
	SDL_Color blue = { 0, 50, 255, 255 };

	SDL_Color color;
	unsigned int selected_pos;
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
	for (unsigned int i = start_pos; i < end_pos; i++) {
		DirObject& n = list[i];
		if (n.selected) {
			color = white;
		}
		else if (n.type == DirObject::FILE) {
			color = green;
		}
		else if (n.type == DirObject::FOLDER) {
			color = blue;
		}
		if (y < dir_box.y + dir_box.h - 20) {
			tex_list.emplace_back(createTextTexture(n.name, color, x, y));
			y += tex_list.back().get()->get_p()->h;
		}
	}
	for (unsigned int i = 0; i < tex_list.size(); i++) {
		renderTexture(tex_list[i].get());
	}
}

void Display::renderUI(ShortcutBar &bar) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &dir_box);
	renderShortcuts(bar);
	renderCurrentPath();
}

void Display::renderShortcuts(ShortcutBar &bar) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &shortcut_box);

	auto &list = bar.get_s();
	for (unsigned int i = 0; i < list.size(); i++) {
		if (list[i].redraw()) {
			buildShortcut(list[i]);
		}
		renderTextureRaw(list[i].texture, list[i].pos);
	}
}

void Display::buildShortcut(Shortcut &shortcut) {
	shortcut.clean();
	shortcut.texture = createTextTextureRaw(shortcut.name, yellow, shortcut.pos);
}
void Display::buildShortcuts(ShortcutBar &bar) {
	int buf = 0;
	for (unsigned int i = 0; i < bar.get_s().size(); i++) {
		bar.get_s()[i].texture = createTextTextureRaw(bar.get_s()[i].name, white, bar.get_s()[i].pos);
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
	auto p = createTextTexture(buf, white, x, y);
	renderTexture(p.get());
}

void Display::renderTexture(Texture* texture) {
	SDL_Rect* destination = texture->get_p();
	SDL_Rect* source = nullptr;
	SDL_RenderCopy(renderer, texture->get_t(), source, destination);
}

void Display::renderTextureRaw(SDL_Texture* tex, SDL_Rect pos) {
	SDL_RenderCopy(renderer, tex, NULL, &pos);
}

SDL_Texture* Display::surfaceToTexture(SDL_Surface* surf) {
	SDL_Texture* text = nullptr;
	text = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);

	if (!text) {
		std::cerr << "Texture Creation Error: " << SDL_GetError() << std::endl;
	}
	return text;
}

SDL_Texture* Display::surfaceToTextureSafe(SDL_Surface* surf) {
	SDL_Texture* text = nullptr;
	text = SDL_CreateTextureFromSurface(renderer, surf);

	if (!text) {
		std::cerr << "Texture Creation Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}
	return text;
}

std::unique_ptr<Texture> Display::createTextTexture(std::string text,
	SDL_Color color,
	int x,
	int y) {
	SDL_Surface* surface = nullptr;
	SDL_Texture* tex = nullptr;
	SDL_Rect pos = { 0, 0, 0, 0 };
	surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (!surface) {
		std::cerr << "Text creationg error" << std::endl;
	}
	pos.w = surface->w;
	pos.h = surface->h;
	pos.x = x;
	pos.y = y;

	if (!surface) {
		std::cerr << "Text Render Error: " << TTF_GetError() << std::endl;
		return nullptr;
	}

	tex = surfaceToTexture(surface);

	auto ptr = std::unique_ptr<Texture>(new Texture(tex, pos));
	return ptr;
}

SDL_Texture* Display::createTextTextureRaw(std::string text,
	SDL_Color color,
	SDL_Rect &pos) {
	SDL_Surface* surface = nullptr;
	SDL_Texture* tex = nullptr;
	surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (!surface) {
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

bool Display::init() {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		success = false;
	}
	else if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		std::cerr << "Linear filtering not enabled!" << std::endl;
	}
	window = SDL_CreateWindow("FileManager",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_W,
		SCREEN_H,
		SDL_WINDOW_SHOWN);
	if (window == NULL) {
		success = false;
	}
	else {
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == nullptr) {
			success = false;
		}
		else {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		}
	}
	if (TTF_Init() == -1) {
		std::cerr << "Failed to initialize TTF: " << SDL_GetError() << std::endl;

		success = false;
	}
	font = TTF_OpenFont("assets/Ubuntu.ttf", 20);

	if (font == NULL) {
		std::cerr << "Failed to load font: " << SDL_GetError() << std::endl;
		success = false;
	}
	return success;
}
