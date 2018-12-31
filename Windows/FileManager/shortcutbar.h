#ifndef SHORTCUT_BAR_H_
#define SHORTCUT_BAR_H_
#include "shortcut.h"
#include <vector>
class ShortcutBar {
public:
	ShortcutBar();
	std::vector<Shortcut>& get_s();
	bool checkClicks(int x, int y, std::string& path);
private:
	std::vector<Shortcut> shortcuts;
};
#endif // SHORTCUT_BAR_H_
