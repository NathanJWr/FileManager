#include "shortcut.h"
Shortcut::Shortcut(std::string n, std::string p,
					int x, int y, int w, int h) {
	name = n;
	path = p;
	pos = { x, y, w, h };
}

void Shortcut::clean() {
	SDL_DestroyTexture(texture);
}
/*
bool Shortcut::clicked(int x, int y) {
	if (x > texture->get_p()->x 
		&& x < texture->get_p()->w 
		&& y > texture->get_p()->y 
		&& y < texture->get_p()->h) {
		if (highlighted) {
			return true;
		} else {
			highlighted = true;
		}
	}
	return false;
}
*/
