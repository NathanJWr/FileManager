#include "shortcut.h"
#include <iostream>
Shortcut::Shortcut(std::string n, std::string p) {
	name = n;
	_path = p;
	highlighted = false;
	changed = false;
}

void Shortcut::clean() {
	SDL_DestroyTexture(texture);
}

bool Shortcut::redraw() {
	return changed;
}

const std::string Shortcut::path() const {
	return _path;
}

bool Shortcut::clicked(int x, int y) {
	if (x >= pos.x
		&& x <= pos.x + pos.w
		&& y >= pos.y
		&& y <= pos.y + pos.h) {
		
		if (highlighted) {
			std::cout << "Clicked" << std::endl;
			changed = true;
			highlighted = false;
			return true;
		} else {
			std::cout << "Highlighted" << std::endl;
			highlighted = true;
			changed = true;
			return false;
		}
	} else {
		changed = false;
		return false;
	}
}