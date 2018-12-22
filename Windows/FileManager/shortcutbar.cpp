#include "shortcutbar.h"
#include "config.h"
#include <windows.h>
#include <Lmcons.h>
ShortcutBar::ShortcutBar() {
	/* Query the OS for username */
	TCHAR username[UNLEN + 1];
	DWORD size = UNLEN + 1;
	GetUserName((TCHAR*)username, &size);

	std::string desktop = ROOT + "Users" + SLASH + username + SLASH + "Desktop";
	std::string documents = ROOT + "Users" + SLASH + username + SLASH + "Documents";

	shortcuts.emplace_back(Shortcut("Desktop", desktop));
	shortcuts.emplace_back(Shortcut("Documents", documents));
}

std::vector<Shortcut>& ShortcutBar::get_s() {
	return shortcuts;
}

bool ShortcutBar::checkClicks(int x, int y, std::string& path) {
	for (Shortcut& n : shortcuts) {
		if (n.clicked(x, y)) {
			path = n.path();
			return true;
		}
	}
	return false;
}
void ShortcutBar::clean() {
	for (auto n : shortcuts) {
		n.clean();
	}
}