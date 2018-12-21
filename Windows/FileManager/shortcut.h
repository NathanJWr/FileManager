#ifndef SHORTCUT_H_
#define SHORTCUT_H_
#include <SDL.h>
#include <string>
class Shortcut {
public:
	Shortcut(std::string n, std::string p, int x, int y, int w, int h);
	bool clicked(int x, int y);
	void clean();

	std::string name;
	SDL_Texture* texture;
	SDL_Rect pos;
private:
	
	std::string path;
	bool highlighted;
};
#endif // SHORTCUT_H_