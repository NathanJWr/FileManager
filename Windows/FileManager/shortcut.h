#ifndef SHORTCUT_H_
#define SHORTCUT_H_
#include <SDL.h>
#include <string>
class Shortcut {
public:
	Shortcut(std::string n, std::string p);
	bool clicked(int x, int y);
	/* if the texture should change to being highlighted or return to normal */
	bool redraw();
	/* free the memory allocated to this object */
	void clean();

	std::string name;
	SDL_Texture* texture;
	SDL_Rect pos;
	bool highlighted;
private:
	std::string path;
	bool changed;
};
#endif // SHORTCUT_H_