#ifndef SHORTCUT_H_
#define SHORTCUT_H_
#include <string>
#include <SDL.h>
class Shortcut {
public:
	Shortcut(std::string n, std::string p);
	/* returns true if the directory should be changed to the Shortcut*/
	bool clicked(int x, int y);
	/* if the texture should change to being highlighted or return to normal */
	bool redraw();

	const std::string path() const;
	const std::string name() const;

	SDL_Rect pos;

	bool highlighted;
private:
	std::string _name;
	std::string _path;
	bool changed;
};
#endif // SHORTCUT_H_